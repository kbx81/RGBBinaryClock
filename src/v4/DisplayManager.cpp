//
// kbx81's binary clock DisplayManager Library
// ---------------------------------------------------------------------------
// (c)2017 by kbx81. See LICENSE for details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//

// The display workflow might be the most confusing thing in this codebase.
// Here is a diagram to help clarify:
// writeDisplay() ->
//   _unadjustedDisplay ->
//     _adjustedDisplay via _refreshAdjustedDisplay() (pre-V4 hardware) ->
//       _updateLedBuffer() / _updateLed() ->
//         _displayDesiredAndSpare[] crossfade buffers ->
//           refresh() which updates LED intensities based on crossfade(s) ->
//             _setDisplayPwmTriad() ->
//               _displayBufferOut ->
//                 DMA to SPI to LED drivers

#include "Display.h"
#include "DisplayManager.h"
#include "RgbLed.h"
#include "TLC59xx.h"


namespace kbxBinaryClock {

namespace DisplayManager
{

// 40-step table for 1/4-step BC via setMasterIntensity(), BC values
//
static const uint8_t cSubBCStepBCTable[41] = {
  0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10 };

// 40-step table for 1/4-step BC via setMasterIntensity(), DC values
//
static const uint8_t cSubBCStepDCTable[41] = {
  0, 32, 64, 95, 127, 50, 75, 95, 127, 95, 106, 116, 127, 103, 111, 119, 127, 108, 114, 121, 127, 111, 116, 122, 127, 114, 118, 122, 127, 115, 119, 123, 127, 116, 120, 123, 127, 117, 121, 124, 127 };

// length of delays used for doubleBlink confirmation
//
static const uint8_t cDoubleBlinkDuration = 50;

// force a full TLC5951 refresh cForceDisplayRefreshInterval calls to refresh()
//
static const uint8_t cForceDisplayRefreshInterval = 250;

// offset in gamma table to trim off some zeros
//
static const uint8_t cGammaLookupOffset = 68;

// maximum value for BC in TLC5951s
//
static const uint16_t cMaxBcValue = 285;

// maximum value for DC in TLC5951s
//
static const uint8_t cMaxDcValue = 127;

// maximum value for gamma-corrected BC in TLC5951s
//
// static const uint16_t cMaxCorrectedBcValue = 1092;
static const uint16_t cMaxCorrectedBcValue = 1220 - cGammaLookupOffset;

// number of TLC59xx devices we're controlling/are linked together
//
static const uint8_t cPwmNumberOfDevices = 3;

// PWM devices
//
static TLC59xx _displayBufferIn(TLC59xx::DeviceType::DeviceTLC5951, cPwmNumberOfDevices, true, TLC59xx::ReversingMode::NoReversing);
static TLC59xx _displayBufferOut(TLC59xx::DeviceType::DeviceTLC5951, cPwmNumberOfDevices, true, TLC59xx::ReversingMode::NoReversing);

// display buffers used for automatically adjusting display intensity
//
static Display _adjustedDisplay, _unadjustedDisplay;

// display buffers for processing crossfades
//
static RgbLed _actualStatusLed, _adjustedStatusLed, _unadjustedStatusLed;
static RgbLed _displayActiveAndStart[2][Display::cPixelCount + 1];
static RgbLed _displayDesiredAndSpare[2][Display::cPixelCount + 1];

// either a 1 or a 0 to indicate buffers in use
//
static uint8_t _activeBufferSet[Display::cPixelCount + 1];

// true if status LED should be refreshed by refresh()
//
static bool _autoRefreshStatusLed = false;

// true if one or more LEDs are in the process of a crossfade
//
static bool _crossfadeInProgress = false;

// if true, BLANK pin will be held high to turn off all LEDs
//
static bool _displayBlank = true;

// if true, last refresh was grayscale data (so next shoud be BcDcFcUd)
//  this ensures that one does not dominate the communication/SPI bus.
static bool _lastRefreshWasGs = false;

// counts calls to refresh() for forced display refreshes
//
static uint8_t _forceDisplayRefreshCounter = 0;

// master display intensity times 100
//   10000 = 100%
static uint16_t _intensityPercentage = cIntensityBaseMultiplier;

// true if it's time to refresh the display
//
volatile static bool _refreshLedIntensitiesNow = false;


// Refreshes the status LED from _actualStatusLed
//
void _refreshStatusLed()
{
  Hardware::setStatusLed(_actualStatusLed);
}


// Modifies a triad of PWM channels in the buffer corresponding to a single RGB LED (pixel)
//
void _setDisplayPwmTriad(const uint8_t ledNumber, const RgbLed &ledValue)
{
  if (ledNumber < Display::cPixelCount)
  {
    _displayBufferOut.setPwmTriad(ledNumber, ledValue.getRed(), ledValue.getGreen(), ledValue.getBlue());
  }
  // status LED
  else if (ledNumber == Display::cPixelCount)
  {
    _actualStatusLed = ledValue;
  }
}


// Updates a single LED in the crossfade buffers
//
void _updateLed(const uint8_t ledNumber, const RgbLed &led)
{
  uint8_t inactiveBufferSet;

  // if ledNumber is valid (including status LED) and if LEDs are different...
  if (ledNumber <= Display::cPixelCount &&
      led != _displayDesiredAndSpare[_activeBufferSet[ledNumber]][ledNumber])
  {
    // ...determine the new buffer to use...
    inactiveBufferSet = (_activeBufferSet[ledNumber] + 1) & 1;
    // put the new LED values into the buffer...
    _displayDesiredAndSpare[inactiveBufferSet][ledNumber] = led;
    // the Start (now Active) LED object contains the fade counter, so reset it.
    _displayActiveAndStart[inactiveBufferSet][ledNumber].setRate(0);
    // ...and update active buffer index
    _activeBufferSet[ledNumber] = inactiveBufferSet;
  }
}


// Updates all main display LEDs in the crossfade buffers
//
void _updateLedBuffer()
{
  uint8_t i;

  // if (_autoAdjustIntensities == true)
  // {
  //   for (i = 0; i < Display::cPixelCount; i++)
  //   {
  //     _updateLed(i, _adjustedDisplay.getLedRaw(i));
  //   }
  // }
  // else
  // {
    for (i = 0; i < Display::cPixelCount; i++)
    {
      _updateLed(i, _unadjustedDisplay.getPixelRaw(i));
    }
  // }
}


// Refreshes _adjustedDisplay
//
void _refreshAdjustedDisplay()
{
  RgbLed workingLed;
  // refresh the values in _adjustedDisplay
  for (uint8_t i = 0; i < Display::cPixelCount; i++)
  {
    workingLed = _unadjustedDisplay.getPixelRaw(i);
    workingLed.adjustIntensity(_intensityPercentage);
    _adjustedDisplay.setPixelFromRaw(i, workingLed);
  }
  // also refresh the values in _adjustedStatusLed if needed
  if (_autoRefreshStatusLed == true)
  {
    workingLed = _unadjustedStatusLed;
    workingLed.adjustIntensity(_intensityPercentage);
    _adjustedStatusLed = workingLed;
    _updateLed(Display::cPixelCount, _adjustedStatusLed);
  }

  _updateLedBuffer();
}


void initialize()
{
  // Initialize the display drivers' dot correction values
  for (uint8_t i = 0; i < Display::cLedCount; i++)
  {
    setDotCorrectionValue(i, 0x7f);
  }
  // Initialize the display drivers' dot correction ranges
  setDotCorrectionRange(true);
  // Initialize the display drivers' global brightness levels
  setMasterIntensity(cIntensityBaseMultiplier);
  // Write the data to the drivers
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversOther, (uint8_t*)_displayBufferIn.getDcBcFcUdBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getDcBcFcUdBufferPtrForWrite(), TLC59xx::cTLC5951ShiftRegSize * cPwmNumberOfDevices, false) == false);
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversGs, (uint8_t*)_displayBufferIn.getPwmBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getPwmBufferPtrForWrite(), TLC59xx::cTLC5951PwmChannelsPerDevice * cPwmNumberOfDevices, true) == false);
}


void refresh()
{
  if (_refreshLedIntensitiesNow == true)
  {
    bool refreshThisLed = false;
    int32_t currentTick, totalTicks, percentTicks;
    uint8_t i, inactiveBufferSet;
    RgbLed activeLed, desiredLed, startLed;

    if (_forceDisplayRefreshCounter++ > cForceDisplayRefreshInterval)
    {
      _forceDisplayRefreshCounter = 0;
      _displayBufferOut.setDcBcFcUdHwRefreshRequired();
      _displayBufferOut.setGsHwRefreshRequired();
    }

    _crossfadeInProgress = false;

    for (i = 0; i <= Display::cPixelCount; i++)
    {
      inactiveBufferSet = (_activeBufferSet[i] + 1) & 1;
      // where we are
      activeLed = _displayActiveAndStart[_activeBufferSet[i]][i];
      // where we started
      startLed = _displayActiveAndStart[inactiveBufferSet][i];
      // where we're going
      desiredLed = _displayDesiredAndSpare[_activeBufferSet[i]][i];
      // where we are (in the fade)
      currentTick = activeLed.getRate();
      // how long we have to get there
      totalTicks = desiredLed.getRate();
      // prevent division by zero
      if (totalTicks == 0)
      {
        totalTicks = 1;
      }

      if (currentTick <= totalTicks)
      {
        _crossfadeInProgress = true;
        refreshThisLed = true;

        percentTicks = (cIntensityBaseMultiplier * currentTick) / totalTicks;

        activeLed.setFromMergedRgbLeds(percentTicks, startLed, desiredLed);
        activeLed.setRate(currentTick + 1);

        _displayActiveAndStart[_activeBufferSet[i]][i] = activeLed;
      }

      if ((refreshThisLed == true) && ((i < Display::cPixelCount) || (_autoRefreshStatusLed == true)))
      {
        activeLed.gammaCorrect12bit();
        _setDisplayPwmTriad(i, activeLed);
        refreshThisLed = false;
      }
    }

    _refreshLedIntensitiesNow = false;
  }

  // if ((_autoAdjustIntensities == true) && (_crossfadeInProgress == false))
  // {
  //   _refreshAdjustedDisplay();
  // }

  if (_displayBufferOut.dcBcFcUdHwRefreshRequired(false) == true)
  {
    if ((_lastRefreshWasGs == true) &&
        (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversOther, (uint8_t*)_displayBufferIn.getDcBcFcUdBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getDcBcFcUdBufferPtrForWrite(), TLC59xx::cTLC5951ShiftRegSize * cPwmNumberOfDevices, false) == true))
    {
      _displayBufferOut.dcBcFcUdHwRefreshRequired(true);

      _lastRefreshWasGs = false;
    }
  }
  else
  {
    _lastRefreshWasGs = false;
  }

  if (_displayBufferOut.gsHwRefreshRequired(false) == true)
  {
    if ((_lastRefreshWasGs == false) &&
        (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversGs, (uint8_t*)_displayBufferIn.getPwmBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getPwmBufferPtrForWrite(), TLC59xx::cTLC5951PwmChannelsPerDevice * cPwmNumberOfDevices, true) == true))
    {
      _displayBufferOut.gsHwRefreshRequired(true);

      _lastRefreshWasGs = true;
    }
  }
  else
  {
    _lastRefreshWasGs = true;
  }

  if ((_autoRefreshStatusLed == true) && (_displayBlank == false))
  {
    _refreshStatusLed();
  }
}


void tick()
{
  _refreshLedIntensitiesNow = true;
}


void setStatusLedAutoRefreshing(const bool autoRefreshEnabled)
{
  _autoRefreshStatusLed = autoRefreshEnabled;
  // update everything if now enabled
  if (autoRefreshEnabled == true)
  {
    _adjustedStatusLed = _unadjustedStatusLed;
    _adjustedStatusLed.adjustIntensity(_intensityPercentage);
    _updateLed(Display::cPixelCount, _adjustedStatusLed);
  }
}


uint16_t getMasterIntensity()
{
  return _intensityPercentage;
}


// 0 - 285: full range...
// 0 - 39: BC & DC lookup in tables
// 40 - 295: BC only, value derived by (value - 29)
//
void setMasterIntensity(const uint16_t intensity)
{
  // v4+ math for BC
  uint32_t _top = cMaxCorrectedBcValue * intensity;
  uint16_t adjustedIntensity = (_top / cIntensityBaseMultiplier) + cGammaLookupOffset,
           safeIntensity = cIntensityBaseMultiplier;
  uint8_t i = 0;
  RgbLed led(adjustedIntensity, adjustedIntensity, adjustedIntensity, 0);

  led.gammaCorrect12bit();

  uint16_t bcValue = led.getRed();
  uint8_t  dcValue = cMaxDcValue;

  // ensure values are good & safe
  if (intensity <= cIntensityBaseMultiplier)
  {
    safeIntensity = intensity;
  }
  else
  {
    bcValue = cMaxBcValue;
  }
  // if the new intensity is different, let's update some stuff
  if (_intensityPercentage != safeIntensity)
  {
    // first, save it so we can check it next time we're called
    _intensityPercentage = safeIntensity;
    // if bcValue is < the size of the lookup table, we'll use the lookup table
    if (bcValue < 41)
    {
      dcValue = cSubBCStepDCTable[bcValue];
      bcValue = cSubBCStepBCTable[bcValue];
    }
    else
    {
      // ...otherwise, just use the computed value - 30...why? the lookup table
      //  covers 41 values, but the BC range covered is 0 to 10, so 41 - 10 == 31
      //  and we throw out the zero value, leaving us with 30.
      bcValue -= 30;
    }
    // now we just write these lovely values into the TLC59xx buffers
    for (i = 0; i < cPwmNumberOfDevices; i++)
    {
      _displayBufferOut.setBcRed(i, bcValue);
      _displayBufferOut.setBcGreen(i, bcValue);
      _displayBufferOut.setBcBlue(i, bcValue);
    }
    // update the display drivers' dot correction values
    for (i = 0; i < Display::cLedCount; i++)
    {
      setDotCorrectionValue(i, dcValue);
    }
    // also refresh the values in _adjustedStatusLed if needed
    if (_autoRefreshStatusLed == true)
    {
      _adjustedStatusLed = _unadjustedStatusLed;
      _adjustedStatusLed.adjustIntensity(_intensityPercentage);
      _updateLed(Display::cPixelCount, _adjustedStatusLed);
    }
  }
}


bool getDisplayBlanking()
{
  return _displayBlank;
}


void setDisplayBlanking(const bool blank)
{
  if (_displayBlank != blank)
  {
    _displayBlank = blank;

    if (blank == true)
    {
      if (_autoRefreshStatusLed == true)
      {
        Hardware::setStatusLed(RgbLed());
      }
      // blank the display (false means enabled with XBLNK pin on drivers)
      Hardware::setDisplayBlankPin(false);
    }
    else
    {
      if (_autoRefreshStatusLed == true)
      {
        _refreshStatusLed();
      }
      // unblank the display (true means disabled with XBLNK pin on drivers)
      Hardware::setDisplayBlankPin(true);
    }
  }
}


void setDotCorrectionRange(const bool useUpperRange)
{
  TLC59xx::DcAdjRange range = TLC59xx::DcAdjRange::DcRange0to66;

  if (useUpperRange == true)
  {
    range = TLC59xx::DcAdjRange::DcRange33to100;
  }

  for (uint8_t i = 0; i < cPwmNumberOfDevices; i++)
  {
    _displayBufferOut.setDcRangeRed(i, range);
    _displayBufferOut.setDcRangeGreen(i, range);
    _displayBufferOut.setDcRangeBlue(i, range);
  }
}


void setDotCorrectionValue(const uint8_t ledNumber, const uint8_t value)
{
  if (ledNumber < Display::cLedCount)
  {
    _displayBufferOut.setDcChannel(ledNumber, value);
  }
}


void doubleBlink()
{
  setDisplayBlanking(true);
  Hardware::delay(cDoubleBlinkDuration);
  setDisplayBlanking(false);
  Hardware::delay(cDoubleBlinkDuration);
  setDisplayBlanking(true);
  Hardware::delay(cDoubleBlinkDuration);
  setDisplayBlanking(false);
}


void writeDisplay(const Display &display)
{
  if (_unadjustedDisplay != display)
  {
    _unadjustedDisplay = display;

    // if (_autoAdjustIntensities == true)
    // {
    //   _refreshAdjustedDisplay();
    // }
    // else
    // {
      _updateLedBuffer();
    // }
  }
}


void writeDisplay(const Display &display, const RgbLed &statusLed)
{
  if (_unadjustedDisplay != display)
  {
    _unadjustedDisplay = display;

    // if (_autoAdjustIntensities == true)
    // {
    //   _refreshAdjustedDisplay();
    // }
    // else
    // {
      _updateLedBuffer();
    // }
  }
  if (_unadjustedStatusLed != statusLed)
  {
    _unadjustedStatusLed = statusLed;

    _adjustedStatusLed = statusLed;
    _adjustedStatusLed.adjustIntensity(_intensityPercentage);
    _updateLed(Display::cPixelCount, _adjustedStatusLed);
  }
}


}

}
