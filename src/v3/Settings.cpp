//
// kbx81's binary clock settings class
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
#include "DateTime.h"
#include "Hardware.h"
#include "RgbLed.h"
#include "Settings.h"


namespace kbxBinaryClock {

  // where settings will be read/written in FLASH
  const uint32_t Settings::cSettingsFlashAddress = 0x0801f000;

  // key used for validating settings
  const uint32_t Settings::cSettingsValidationKey = 0x55aa55aa;

  // conatins a mask for bitfields and a maximum value for other types of data
  const uint16_t Settings::cSettingData[] = { 0x03ff,   // SystemOptions
                                              0x00ff,   // BeepStates
                                              0x00ff,   // BlinkStates
                                              0x00ff,   // ColorStates
                                              300,      // TimeDisplayDuration
                                              300,      // DateDisplayDuration
                                              300,      // TemperatureDisplayDuration
                                              1000,     // FadeRate
                                              12,       // DstBeginMonth
                                              4,        // DstBeginDowOrdinal
                                              12,       // DstEndMonth
                                              4,        // DstEndDowOrdinal
                                              6,        // DstSwitchDayOfWeek
                                              23,       // DstSwitchHour
                                              1000,     // MinimumIntensity
                                              4095,     // FlickerReduction
                                              3,        // CurrentDrive
                                              31,       // TemperatureCalibration
                                              7,        // BeeperVolume
                                              512 - 72 }; // DmxAddress

  Settings::Settings()
  : _validityKey(cSettingsValidationKey)
  {
    initialize();
  }


  void Settings::initialize()
  {
    uint16_t intensity = 4095,
             rate = 100;
    uint8_t  i = 0;
    RgbLed   defaultRed(intensity, 0, 0, rate),
             defaultOrange(intensity, intensity / 3, 0, rate),
             defaultYellow(intensity, intensity, 0, rate),
             defaultGreen(0, intensity, 0, rate),
             defaultCyan(0, intensity, intensity, rate),
             defaultBlue(0, 0, intensity, rate),
             defaultViolet(intensity / 8, 0, intensity, rate),
             defaultMagenta(intensity, 0, intensity, rate),
             defaultWhite(intensity, intensity, intensity, rate),
             defaultGray(intensity / 12, intensity / 12, intensity / 12, rate),
             defaultOff(0, 0, 0, rate);

    // set default time slots every three hours starting at 0100 hours
    for (i = static_cast<uint8_t>(Slot::Slot1); i <= static_cast<uint8_t>(Slot::Slot8); i++)
    {
      _time[i].setTime((3 * i) + 1, 30, 0);
    }

    _setting[static_cast<uint8_t>(SystemOptions)] = 0x2c1;
    _setting[static_cast<uint8_t>(BeepStates)] = 0x00;
    _setting[static_cast<uint8_t>(BlinkStates)] = 0x00;
    _setting[static_cast<uint8_t>(ColorStates)] = 0xff;
    _setting[static_cast<uint8_t>(TimeDisplayDuration)] = 24;
    _setting[static_cast<uint8_t>(DateDisplayDuration)] = 3;
    _setting[static_cast<uint8_t>(TemperatureDisplayDuration)] = 3;
    _setting[static_cast<uint8_t>(FadeRate)] = 100;
    _setting[static_cast<uint8_t>(DstBeginMonth)] = 3;
    _setting[static_cast<uint8_t>(DstBeginDowOrdinal)] = 2;
    _setting[static_cast<uint8_t>(DstEndMonth)] = 11;
    _setting[static_cast<uint8_t>(DstEndDowOrdinal)] = 1;
    _setting[static_cast<uint8_t>(DstSwitchDayOfWeek)] = 0;
    _setting[static_cast<uint8_t>(DstSwitchHour)] = 2;
    _setting[static_cast<uint8_t>(MinimumIntensity)] = 30;
    _setting[static_cast<uint8_t>(FlickerReduction)] = 800;
    _setting[static_cast<uint8_t>(CurrentDrive)] = 0;
    _setting[static_cast<uint8_t>(TemperatureCalibration)] = 10;
    _setting[static_cast<uint8_t>(BeeperVolume)] = 7;
    _setting[static_cast<uint8_t>(DmxAddress)] = 0;

    _color0[Slot::Slot1] = defaultRed;    // 0130
    _color1[Slot::Slot1] = defaultOrange;
    _color0[Slot::Slot2] = defaultRed;    // 0430
    _color1[Slot::Slot2] = defaultOrange;
    _color0[Slot::Slot3] = defaultOrange; // 0730
    _color1[Slot::Slot3] = defaultYellow;
    _color0[Slot::Slot4] = defaultYellow; // 1030
    _color1[Slot::Slot4] = defaultMagenta;
    _color0[Slot::Slot5] = defaultCyan;   // 1330 / 1:30 pm
    _color1[Slot::Slot5] = defaultYellow;
    _color0[Slot::Slot6] = defaultOrange; // 1630 / 4:30 pm
    _color1[Slot::Slot6] = defaultMagenta;
    _color0[Slot::Slot7] = defaultBlue;   // 1930 / 7:30 pm
    _color1[Slot::Slot7] = defaultOrange;
    _color0[Slot::Slot8] = defaultRed;    // 2230 / 10:30 pm
    _color1[Slot::Slot8] = defaultOrange;
    _color0[Slot::SlotDate] = defaultGray;
    _color1[Slot::SlotDate] = defaultWhite;
    _color0[Slot::SlotTemperature] = defaultBlue;
    _color1[Slot::SlotTemperature] = defaultYellow;
    _color0[Slot::SlotTimer] = defaultBlue;
    _color1[Slot::SlotTimer] = defaultWhite;
    _color0[Slot::SlotMenu] = defaultGray;
    _color1[Slot::SlotMenu] = defaultWhite;
    _color0[Slot::SlotSet] = defaultGreen;
    _color1[Slot::SlotSet] = defaultRed;
    _color0[Slot::SlotDmx] = defaultOff;
    _color1[Slot::SlotDmx] = defaultGray;
    _color0[Slot::SlotCalculated] = defaultGreen;
    _color1[Slot::SlotCalculated] = defaultRed;

    _color0[Slot::SlotMenu].setRate(0);
    _color1[Slot::SlotMenu].setRate(0);
    _color0[Slot::SlotSet].setRate(0);
    _color1[Slot::SlotSet].setRate(0);

    _validityKey = cSettingsValidationKey;  // settings are now valid
}


  void Settings::loadFromFlash()
  {
    RgbLed orange(1024, 120, 0, 0),
           green(0, 768, 0, 0);

    Hardware::readFlash(cSettingsFlashAddress, sizeof(Settings), (uint8_t*)this);

    if (_validityKey != cSettingsValidationKey)
    {
      initialize();
      // blink to alert that settings could not be loaded
      Hardware::blinkStatusLed(green, orange, 6, 120000);
    }
  }


  void Settings::saveToFlash()
  {
    while (Hardware::writeFlash(cSettingsFlashAddress, (uint8_t*)this, sizeof(Settings)) != 0);
  }


  bool Settings::getSetting(const uint8_t setting, const uint8_t item)
  {
    if (setting <= static_cast<uint8_t>(Setting::DmxAddress))
    {
      return (_setting[setting] & (1 << item));
    }
    return false;
  }


  void Settings::setSetting(const uint8_t setting, const uint8_t item, const bool state)
  {
    if (setting <= static_cast<uint8_t>(Setting::DmxAddress))
    {
      if (state)
      {
        _setting[setting] |= (1 << item);
      }
      else
      {
        _setting[setting] &= ~(1 << item);
      }
    }
  }


  uint16_t Settings::getRawSetting(const uint8_t setting)
  {
    if (setting <= static_cast<uint8_t>(Setting::DmxAddress))
    {
      return _setting[setting];
    }
    return 0;
  }


  void Settings::setRawSetting(const uint8_t setting, const uint16_t value)
  {
    if (setting <= static_cast<uint8_t>(Setting::DmxAddress))
    {
      _setting[setting] = value;
    }
  }


  DateTime Settings::getTime(const Slot setting)
  {
    return getTime(static_cast<uint8_t>(setting));
  }


  DateTime Settings::getTime(const uint8_t setting)
  {
    if (setting <= static_cast<uint8_t>(Slot::Slot8))
    {
      return _time[static_cast<uint8_t>(setting)];
    }
    return _time[static_cast<uint8_t>(Slot::Slot1)];
  }


  void Settings::setTime(const Slot setting, const DateTime time)
  {
    return setTime(static_cast<uint8_t>(setting), time);
  }


  void Settings::setTime(const uint8_t setting, const DateTime time)
  {
    if (setting <= static_cast<uint8_t>(Slot::Slot8))
    {
      _time[static_cast<uint8_t>(setting)] = time;
    }
  }


  RgbLed Settings::getColor0(const Slot slot)
  {
    return _color0[static_cast<uint8_t>(slot)];
  }


  RgbLed Settings::getColor0(const uint8_t slot)
  {
    if (slot <= static_cast<uint8_t>(Slot::SlotCalculated))
    {
      return _color0[static_cast<uint8_t>(slot)];
    }
    return _color0[static_cast<uint8_t>(Slot::Slot1)];
  }


  RgbLed Settings::getColor1(const Slot slot)
  {
    return _color1[static_cast<uint8_t>(slot)];
  }


  RgbLed Settings::getColor1(const uint8_t slot)
  {
    if (slot <= static_cast<uint8_t>(Slot::SlotCalculated))
    {
      return _color1[static_cast<uint8_t>(slot)];
    }
    return _color1[static_cast<uint8_t>(Slot::Slot1)];
  }


  void Settings::setColors(const Slot slot, const RgbLed color0, const RgbLed color1)
  {
    setColors(static_cast<uint8_t>(slot), color0, color1);
  }


  void Settings::setColors(const uint8_t slot, const RgbLed color0, const RgbLed color1)
  {
    if ((slot <= static_cast<uint8_t>(Slot::SlotDmx)))
    {
      _color0[slot] = color0;
      _color1[slot] = color1;
    }
  }


  void Settings::refreshCalculatedColors()
  {
    DateTime currentTime = Hardware::getDateTime();
    RgbLed   defaultWhite(4095, 4095, 4095, _setting[static_cast<uint8_t>(FadeRate)]),
             defaultDarkWhite(512, 512, 512, _setting[static_cast<uint8_t>(FadeRate)]);
    int32_t i, delta, percentage, secondsSinceMidnight,
            // minSecs = 0,  // first/earliest slot
            prevSecs = 0, // slot most recently passed
            nextSecs = 0, // next slot that will pass
            maxSecs = 0;  // latest/last slot
    uint8_t // minSlot = 0,  // index of first/earliest slot
            prevSlot = 0, // index of slot most recently passed
            nextSlot = 0, // index of next slot that will pass
            maxSlot = 0;  // index of latest/last slot

    secondsSinceMidnight = (int32_t)currentTime.secondsSinceMidnight(false);

    // we could set the rate, too, but other functions don't, so...
    // _color0[Slot::SlotCalculated].setRate(_setting[static_cast<uint8_t>(FadeRate)]);
    // _color1[Slot::SlotCalculated].setRate(_setting[static_cast<uint8_t>(FadeRate)]);

    if (Hardware::rtcIsSet() == true)
    {
      // find minimum, maximum, just passed, and next to pass values
      for (i = static_cast<uint8_t>(Slot::Slot1); i <= static_cast<uint8_t>(Slot::Slot8); i++)
      {
        if (_setting[static_cast<uint8_t>(Setting::ColorStates)] & (1 << i))
        {
          delta = (int32_t)_time[i].secondsSinceMidnight(false) - secondsSinceMidnight;

          // if (((int32_t)_time[i].secondsSinceMidnight(false) < minSecs) || !i)
          // {
          //   minSecs = _time[i].secondsSinceMidnight(false);
          //   minSlot = i;
          // }

          if (delta - 1 < 0 && ((delta - 1 > prevSecs) || !i))
          {
            prevSecs = delta - 1;
            prevSlot = i;
          }

          if (delta > 0 && ((delta < nextSecs) || !nextSecs))
          {
            nextSecs = delta;
            nextSlot = i;
          }

          if (((int32_t)_time[i].secondsSinceMidnight(false) > maxSecs) || !i)
          {
            maxSecs = _time[i].secondsSinceMidnight(false);
            maxSlot = i;
          }
        }
      }
      // if no slots have been passed yet, then the last one passed is the max/latest one
      if (prevSecs >= 0)
      {
        prevSlot = maxSlot;
      }

      if ((_setting[static_cast<uint8_t>(Setting::SystemOptions)] & SystemOptionsBits::FadeAcrossSlots)
          && _setting[static_cast<uint8_t>(Setting::ColorStates)])
      {
        // for each color (R, G, B), compute the value between old and new
        // first, if prev > current time, add 86400 to current
        if ((int32_t)_time[prevSlot].secondsSinceMidnight(false) > secondsSinceMidnight)
        {
          secondsSinceMidnight += 86400;
        }
        // next, if next - prev < 0, add 86400 to next
        if (_time[nextSlot].secondsSinceMidnight(false) < _time[prevSlot].secondsSinceMidnight(false))
        {
          nextSecs = _time[nextSlot].secondsSinceMidnight(false) + 86400;
        }
        else
        {
          nextSecs = _time[nextSlot].secondsSinceMidnight(false);
        }
        //  ...finally: (current - prev) / (next - prev) = % to new value
        percentage = 10000 * (secondsSinceMidnight - _time[prevSlot].secondsSinceMidnight(false));
        percentage /= (nextSecs - _time[prevSlot].secondsSinceMidnight(false));

        _color0[Slot::SlotCalculated].mergeRgbLeds(percentage, _color0[prevSlot], _color0[nextSlot]);
        _color1[Slot::SlotCalculated].mergeRgbLeds(percentage, _color1[prevSlot], _color1[nextSlot]);
      }
      else
      {
        _color0[Slot::SlotCalculated] = _color0[prevSlot];
        _color1[Slot::SlotCalculated] = _color1[prevSlot];
      }
    }
    else
    {
      _color0[Slot::SlotCalculated] = defaultDarkWhite;
      _color1[Slot::SlotCalculated] = defaultWhite;
    }
  }


}
