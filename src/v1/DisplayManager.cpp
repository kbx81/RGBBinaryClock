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

// As hardware version 4+ uses the TLC5951 drivers, "master" dimming is
// implemented at the hardware level by leveraging the global brightness control
// (BC) and dot control (DC) (at low levels) mechanisms. This makes crossfading
// at low intensity levels more smooth and also reduces the workload on the CPU.
//
// That said, the display workflow might be the most confusing aspect of this
// codebase. The following flow diagram should help clarify how it works:
//
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
#include "Hardware.h"
#include "RgbCrossfader.h"
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
static const uint16_t cForceDisplayRefreshInterval = 1000;

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
static const uint16_t cMaxCorrectedBcValue = 1220 - cGammaLookupOffset;

// number of TLC59xx devices we're controlling/are linked together
//
static const uint8_t cPwmNumberOfDevices = 3;

// PWM devices
//
static TLC59xx _displayBufferIn(TLC59xx::DeviceType::DeviceTLC5951, cPwmNumberOfDevices, true, TLC59xx::ReversingMode::NoReversing);
static TLC59xx _displayBufferOut(TLC59xx::DeviceType::DeviceTLC5951, cPwmNumberOfDevices, true, TLC59xx::ReversingMode::NoReversing);

// display buffers for processing crossfades
//
static RgbLed _statusLed;

// LED crossfaders (main display + status LED)
//
static RgbCrossfader _crossfader[Display::cPixelCount + 1];

// true if status LED should be refreshed by refresh()
//
static bool _autoRefreshStatusLed = false;

// if true, BLANK pin will be held high to turn off all LEDs
//
static bool _displayBlank = true;

// if true, last refresh was grayscale data (so next shoud be BcDcFcUd)
//  this ensures that one does not dominate communication on the SPI bus.
static bool _lastRefreshWasGs = false;

// counts calls to refresh() for forced display refreshes
//
static uint16_t _forceDisplayRefreshCounter = 0;

// master display intensity times 100
//   10000 = 100%
static uint16_t _intensityPercentage = RgbLed::cLed100Percent;

// true if it's time to refresh the display
//
volatile static bool _refreshLedIntensitiesNow = false;


// Modifies a triad of PWM channels in the buffer corresponding to a single RGB LED (pixel)
//
void _setDisplayPwmTriad(const uint8_t ledNumber, const RgbLed &ledValue)
{
  if (ledNumber < Display::cPixelCount)
  {
    if (Hardware::cTargetHardwareVersion >= 4)
    {
      _displayBufferOut.setPwmTriad(ledNumber, ledValue.getRed(), ledValue.getGreen(), ledValue.getBlue());
    }
    else
    {
      _displayBufferOut.setPwmChannel(ledNumber, ledValue.getRed());
      _displayBufferOut.setPwmChannel(ledNumber + 24, ledValue.getGreen());
      _displayBufferOut.setPwmChannel(ledNumber + 48, ledValue.getBlue());
    }
  }
  // status LED
  else if (ledNumber == Display::cPixelCount)
  {
    _statusLed = ledValue;
  }
}


void initialize()
{
  // Initialize the display drivers' global brightness levels
  setMasterIntensity(RgbLed::cLed100Percent);

  if (Hardware::cTargetHardwareVersion >= 4)
  {
    // Initialize the display drivers' dot correction values
    for (uint8_t i = 0; i < Display::cLedCount; i++)
    {
      setDotCorrectionValue(i, 0x7f);
    }
    // Initialize the display drivers' dot correction ranges
    setDotCorrectionRange(true);
    // Write the BC, DC, FC, and UD to the drivers
    while (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversOther, (uint8_t*)_displayBufferIn.getDcBcFcUdBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getDcBcFcUdBufferPtrForWrite(), TLC59xx::cTLC5951ShiftRegSize * cPwmNumberOfDevices, false) == false);
  }
  else
  {
    _displayBufferIn.setReversingMode(TLC59xx::ReversingMode::ReverseAcrossBuffer);
    _displayBufferOut.setReversingMode(TLC59xx::ReversingMode::ReverseAcrossBuffer);
  }
  // Write the GS data to the drivers
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::LedDriversGs, (uint8_t*)_displayBufferIn.getPwmBufferPtrForWrite(), (uint8_t*)_displayBufferOut.getPwmBufferPtrForWrite(), TLC59xx::cTLC5951PwmChannelsPerDevice * cPwmNumberOfDevices, true) == false);
}


void refresh()
{
  if (_refreshLedIntensitiesNow == true)
  {
    RgbLed activeLed;

    // for v4+ hardware, we force periodic display refreshes to ensure a valid
    //  display at all times since fingers on the PCB can wreak havoc ;)
    if ((Hardware::cTargetHardwareVersion >= 4)
        && (_forceDisplayRefreshCounter++ > cForceDisplayRefreshInterval))
    {
      _forceDisplayRefreshCounter = 0;
      _displayBufferOut.setDcBcFcUdHwRefreshRequired();
      _displayBufferOut.setGsHwRefreshRequired();
    }

    // refresh all crossfaders, adjust intensities (pre-v4), write PWM buffer
    for (uint8_t i = 0; i <= Display::cPixelCount; i++)
    {
      _crossfader[i].tick();

      activeLed = _crossfader[i].getActive();
      if ((Hardware::cTargetHardwareVersion <= 3) || (i == Display::cPixelCount))
      {
        activeLed.adjustIntensity(_intensityPercentage);
      }
      activeLed.gammaCorrect12bit();
      _setDisplayPwmTriad(i, activeLed);
    }

    _refreshLedIntensitiesNow = false;  // all done...until the next tick...
  }

  // for v4+ hardware, check if DC, BC, FC, & UD need refreshing and do it if so
  if ((Hardware::cTargetHardwareVersion >= 4)
      && (_displayBufferOut.dcBcFcUdHwRefreshRequired(false) == true))
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

  // check if GS data needs refreshing and do it if so
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
  // don't forget about the lovely status LED!
  if ((_autoRefreshStatusLed == true) && (_displayBlank == false))
  {
    Hardware::setStatusLed(_statusLed);
  }
}


void tick()
{
  _refreshLedIntensitiesNow = true;
}


void setStatusLedAutoRefreshing(const bool autoRefreshEnabled)
{
  _autoRefreshStatusLed = autoRefreshEnabled;
}


uint16_t getMasterIntensity()
{
  return _intensityPercentage;
}


void setMasterIntensity(const uint16_t intensity)
{
  // this is a bit deceiving as it makes v4+ seem more complicated with respect
  //  to "master" dimming. yes, we spend a few more cycles computing BC and DC
  //  values for the TLC5951s, but it's significantly fewer cycles than doing a
  //  brightness adjustment operation for every single LED (all 75 of them) with
  //  every display refresh. hardware dimming ultimately saves a lot of cycles.
  uint16_t safeIntensity = RgbLed::cLed100Percent;

  if (Hardware::cTargetHardwareVersion >= 4)
  {
    // v4+ math for TLC5951 BC
    uint32_t _top = cMaxCorrectedBcValue * intensity;
    uint16_t adjustedIntensity = (_top / RgbLed::cLed100Percent) + cGammaLookupOffset;
    uint8_t i = 0;
    RgbLed led(adjustedIntensity, adjustedIntensity, adjustedIntensity, 0);

    led.gammaCorrect12bit();

    uint16_t bcValue = led.getRed();
    uint8_t  dcValue = cMaxDcValue;

    // ensure values are good & safe
    if (intensity <= RgbLed::cLed100Percent)
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
      // if bcValue is less than the size of the lookup table, use the table
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
    }
  }
  else
  {
    // ensure values are good & safe
    if (intensity <= RgbLed::cLed100Percent)
    {
      safeIntensity = intensity;
    }
    // if the new intensity is different, let's update some stuff
    if (_intensityPercentage != safeIntensity)
    {
      // first, save it so we can check it next time we're called
      _intensityPercentage = safeIntensity;
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
      // blank the display
      Hardware::setDisplayHardwareBlanking(true);
    }
    else
    {
      if (_autoRefreshStatusLed == true)
      {
        Hardware::setStatusLed(_statusLed);
      }
      // unblank the display
      Hardware::setDisplayHardwareBlanking(false);
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


// Updates all main display LEDs in the crossfade buffers
//
void _loadCrossfaders(const Display &display)
{
  for (uint8_t i = 0; i < Display::cPixelCount; i++)
  {
    _crossfader[i].startNewFadeIfDifferent(display.getPixelRaw(i));
  }
}


void writeDisplay(const Display &display)
{
  _loadCrossfaders(display);
}


void writeDisplay(const Display &display, const RgbLed &statusLed)
{
  _loadCrossfaders(display);

  _crossfader[Display::cPixelCount].startNewFadeIfDifferent(statusLed);
}


}

}
