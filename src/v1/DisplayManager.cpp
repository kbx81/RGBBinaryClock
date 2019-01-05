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
// (BC) and dot control (DC) mechanisms. This makes crossfading at (very) low
// intensity levels smoother and also reduces the workload on the CPU.
//
// That said, the display workflow might be the most confusing aspect of this
// codebase. The following flow diagram should help clarify how it works:
//
// writeDisplay()/writeStatusLed() ->
//   _loadCrossfaders() ->
//     refresh() updates crossfades when _refreshLedIntensitiesNow is set via tick() ->
//       _setDisplayPwmTriad() ->
//         _displayBufferOut (TCL59xx PWM data buffer) ->
//           DMA to SPI to TLC59xx shift registers & latches

#include "Display.h"
#include "DisplayManager.h"
#include "Hardware.h"
#include "RgbCrossfader.h"
#include "RgbLed.h"
#include "TLC59xx.h"


namespace kbxBinaryClock {

namespace DisplayManager
{

// value of TLC5951 Iref resistors in ohms; update this if yours are different
//  this is not relevant for pre-v4 boards with TLC5947 ICs
static const uint16_t cIrefOhms = 2700;

// TLC5951 IolcMax (current through LEDs) * 1000000 (for precision, yay)
//
static const uint32_t cIolcMax = ((1.2 * 40) * 1000000) / cIrefOhms;

// length of delays used for doubleBlink confirmation; passed to Hardware::delay()
//
static const uint8_t cDoubleBlinkDuration = 50;

// force a full TLC5951 refresh cForceDisplayRefreshInterval calls to refresh()
//
static const uint16_t cForceDisplayRefreshInterval = 1000;

// maximum value for BC in TLC5951s
//
static const uint16_t cMaxBcValue = 255;

// maximum value for DC in TLC5951s
//
static const uint8_t cMaxDcValue = 127;

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
static uint16_t _intensityPercentage = 0;

// set by tick() to true so refresh() will refresh the RgbCrossfaders
//
volatile static bool _refreshLedIntensitiesNow = false;

// counter for the display refresh interval
//
static uint8_t _driverRefreshCounter = 0;

// TLC59xx refresh interval; slows down refreshing of the drivers to reduce
//  flicker on boards with TLC5947 ICs. not useful for the TLC5951s.
static uint8_t _driverRefreshInterval = 0;


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
  // Initialize the display drivers' global brightness and dot control levels
  setMasterIntensity(RgbLed::cLed100Percent);

  if (Hardware::cTargetHardwareVersion >= 4)
  {
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
      // we always have to "soft" adjust the status LED, hence the ||
      if ((Hardware::cTargetHardwareVersion <= 3) || (i == Display::cPixelCount))
      {
        activeLed.adjustIntensity(_intensityPercentage);
      }
      activeLed.gammaCorrect12bit();
      _setDisplayPwmTriad(i, activeLed);
    }

    _refreshLedIntensitiesNow = false;  // all done...until the next tick...

    _driverRefreshCounter++;
  }

  if (_driverRefreshCounter >= _driverRefreshInterval)
  {
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

    // reset _driverRefreshCounter as appropriate
    if (Hardware::cTargetHardwareVersion <= 3)
    {
      if (_displayBufferOut.gsHwRefreshRequired(false) == false)
      {
        _driverRefreshCounter = 0;
      }
    }
    else
    {
      if ((_displayBufferOut.dcBcFcUdHwRefreshRequired(false) == false) && (_displayBufferOut.gsHwRefreshRequired(false) == false))
      {
        _driverRefreshCounter = 0;
      }
    }
  }
}


void tick()
{
  _refreshLedIntensitiesNow = true;
  // refresh the status LED here to play nicely with strobing via DMX-512
  if (_autoRefreshStatusLed == true)
  {
    if (_displayBlank == false)
    {
      Hardware::setStatusLed(_statusLed);
    }
    else
    {
      Hardware::setStatusLed(RgbLed());
    }
  }
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
  uint16_t safeIntensity = RgbLed::cLed100Percent;

  // ensure the passed intensity value is appropriate & safe
  if (intensity < RgbLed::cLed100Percent)
  {
    safeIntensity = intensity;
  }

  if (Hardware::cTargetHardwareVersion >= 4)
  {
    // this is a bit deceiving as it makes v4+ seem more complex with respect to
    //  "master" dimming. we'll spend a few more cycles here computing BC and DC
    //  values for the TLC5951s, but it's significantly fewer cycles than doing
    //  a brightness adjustment operation for every single LED (all 75 of them)
    //  with every display refresh. hardware dimming ultimately saves a
    //  significant number of cycles.
    uint32_t bcValue = cMaxBcValue,
             dcValue = cMaxDcValue,
             iOutDesired = cIolcMax;
    uint8_t i = 0;

    // if the new intensity is different, let's update some stuff
    if (_intensityPercentage != safeIntensity)
    {
      // first, save it so we can check it next time we're called
      _intensityPercentage = safeIntensity;

      // gamma correct it -- the big & slow but correct way (requires <math.h>)
      // if (safeIntensity < RgbLed::cLed100Percent)
      // {
      //   safeIntensity = (pow((double)safeIntensity / (double)RgbLed::cLed100Percent, gamma) * RgbLed::cLed100Percent) + 0.5;
      // }

      // gamma correct it -- the quick, dirty, and lower-resolution way
      safeIntensity = (safeIntensity * Display::cLedMaxIntensity) / RgbLed::cLed100Percent;
      RgbLed gamma(safeIntensity, safeIntensity, safeIntensity);
      gamma.gammaCorrect12bit();        // this is just to use the lookup table
      safeIntensity = gamma.getRed();   // ...or any other color as set above

      // the formulas below were derived directly from the TLC5951 reference manual.
      // use _intensityPercentage as a percentage of cIolcMax and compute BC & DC from that
      iOutDesired = (safeIntensity * cIolcMax) / RgbLed::cLed100Percent;

      if (iOutDesired > 0)
      {
        // compute value for BC; always add one so it exceeds what we need...
        bcValue = ((iOutDesired * cMaxBcValue) / cIolcMax) + 1;
        if (bcValue > cMaxBcValue)  // was it too big?
        {
          // set bcValue to the max; dcValue remains as initialized. full brightness!
          bcValue = cMaxBcValue;
        }
        else
        {
          // compute the DC value to scale down from what we calculated above.
          dcValue = (iOutDesired * cMaxBcValue * cMaxDcValue) / (bcValue * cIolcMax);
        }
      }
      else  // it was zero so we want "off"
      {
        bcValue = 0;
        dcValue = 0;
      }

      // now we just write these lovely values into the TLC59xx buffers
      for (i = 0; i < cPwmNumberOfDevices; i++)
      {
        _displayBufferOut.setBcRed(i, bcValue);
        _displayBufferOut.setBcGreen(i, bcValue);
        _displayBufferOut.setBcBlue(i, bcValue);
      }
      // ...update the display drivers' dot correction values, too
      for (i = 0; i < Display::cLedCount; i++)
      {
        setDotCorrectionValue(i, dcValue);
      }
    }
  }
  else
  {
    // if the new intensity is different, let's update some stuff
    if (_intensityPercentage != safeIntensity)
    {
      // save it...we'll need it later for sure
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


uint8_t getDisplayRefreshInterval()
{
  return _driverRefreshInterval;
}


void setDisplayRefreshInterval(const uint8_t interval)
{
  _driverRefreshInterval = interval;
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


void writeStatusLed(const RgbLed &statusLed)
{
  _crossfader[Display::cPixelCount].startNewFadeIfDifferent(statusLed);
}


}

}
