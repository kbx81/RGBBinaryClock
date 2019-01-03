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
#include "Application.h"
#include "DateTime.h"
#include "Hardware.h"
#include "RgbLed.h"
#include "Settings.h"


namespace kbxBinaryClock {

  // where settings will be read/written in FLASH
  const uint32_t Settings::cSettingsFlashAddress = 0x0801f000;

  // conatins a mask for bitfields and a maximum value for other types of data
  const uint16_t Settings::cSettingData[] = { 0x07ff,   // SystemOptions
                                              0x03ff,   // BeepStates
                                              0x03ff,   // BlinkStates
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
                                              65535,    // TimerResetValue
                                              100,      // DisplayRefreshInterval
                                              512 - 72 }; // DmxAddress

  Settings::Settings()
  : _crc(0)
  {
    initialize();
  }


  void Settings::initialize()
  {
    // set default time slots every three hours starting at 0100 hours
    for (uint8_t i = static_cast<uint8_t>(Slot::Slot1); i <= static_cast<uint8_t>(Slot::Slot8); i++)
    {
      _time[i].setTime((3 * i) + 1, 30, 0);
    }

    _setting[static_cast<uint8_t>(SystemOptions)] = 0x2c1;
    _setting[static_cast<uint8_t>(BeepStates)] = 0x300;
    _setting[static_cast<uint8_t>(BlinkStates)] = 0x000;
    _setting[static_cast<uint8_t>(ColorStates)] = 0xff;
    _setting[static_cast<uint8_t>(TimeDisplayDuration)] = 24;
    _setting[static_cast<uint8_t>(DateDisplayDuration)] = 3;
    _setting[static_cast<uint8_t>(TemperatureDisplayDuration)] = 3;
    _setting[static_cast<uint8_t>(FadeDuration)] = 100;
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
    _setting[static_cast<uint8_t>(TimerResetValue)] = 30;
    _setting[static_cast<uint8_t>(DisplayRefreshInterval)] = 0;
    _setting[static_cast<uint8_t>(DmxAddress)] = 0;

    _color[Color::Color0][Slot::Slot1] = Application::red;    // 0130
    _color[Color::Color1][Slot::Slot1] = Application::orange;
    _color[Color::Color0][Slot::Slot2] = Application::red;    // 0430
    _color[Color::Color1][Slot::Slot2] = Application::orange;
    _color[Color::Color0][Slot::Slot3] = Application::orange; // 0730
    _color[Color::Color1][Slot::Slot3] = Application::yellow;
    _color[Color::Color0][Slot::Slot4] = Application::yellow; // 1030
    _color[Color::Color1][Slot::Slot4] = Application::magenta;
    _color[Color::Color0][Slot::Slot5] = Application::cyan;   // 1330 / 1:30 pm
    _color[Color::Color1][Slot::Slot5] = Application::yellow;
    _color[Color::Color0][Slot::Slot6] = Application::orange; // 1630 / 4:30 pm
    _color[Color::Color1][Slot::Slot6] = Application::magenta;
    _color[Color::Color0][Slot::Slot7] = Application::blue;   // 1930 / 7:30 pm
    _color[Color::Color1][Slot::Slot7] = Application::orange;
    _color[Color::Color0][Slot::Slot8] = Application::red;    // 2230 / 10:30 pm
    _color[Color::Color1][Slot::Slot8] = Application::orange;
    _color[Color::Color0][Slot::SlotDate] = Application::gray;
    _color[Color::Color1][Slot::SlotDate] = Application::white;
    _color[Color::Color0][Slot::SlotTemperature] = Application::blue;
    _color[Color::Color1][Slot::SlotTemperature] = Application::yellow;
    _color[Color::Color0][Slot::SlotTimer] = Application::blue;
    _color[Color::Color1][Slot::SlotTimer] = Application::white;
    _color[Color::Color0][Slot::SlotMenu] = Application::gray;
    _color[Color::Color1][Slot::SlotMenu] = Application::white;
    _color[Color::Color0][Slot::SlotSet] = Application::green;
    _color[Color::Color1][Slot::SlotSet] = Application::red;
    _color[Color::Color0][Slot::SlotDmx] = Application::gray;
    _color[Color::Color1][Slot::SlotDmx] = Application::white;
    _color[Color::Color0][Slot::SlotCalculated] = Application::green;
    _color[Color::Color1][Slot::SlotCalculated] = Application::red;

    // _color[Color::Color0][Slot::SlotMenu].setDuration(0);
    // _color[Color::Color1][Slot::SlotMenu].setDuration(0);
    // _color[Color::Color0][Slot::SlotSet].setDuration(0);
    // _color[Color::Color1][Slot::SlotSet].setDuration(0);
}


  bool Settings::loadFromFlash()
  {
    Hardware::readFlash(cSettingsFlashAddress, sizeof(Settings), (uint8_t*)this);

    uint32_t loadedCrc = _crc;  // save before zeroing for the CRC computation

    _crc = 0;   // always compute the CRC with this as zero

    if (loadedCrc != Hardware::getCRC((uint32_t*)this, sizeof(Settings) / 4))
    {
      initialize();

      return false;
    }
    return true;
  }


  uint32_t Settings::saveToFlash()
  {
    _crc = 0;   // always compute the CRC with this as zero

    // compute the new CRC value and save it before we write it all to FLASH
    _crc = Hardware::getCRC((uint32_t*)this, sizeof(Settings) / 4);

    return Hardware::writeFlash(cSettingsFlashAddress, (uint8_t*)this, sizeof(Settings));
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


  RgbLed Settings::getColor(const Color color, const Slot slot, const bool setFadeDuration)
  {
    return getColor(static_cast<uint8_t>(color), static_cast<uint8_t>(slot), setFadeDuration);
  }


  RgbLed Settings::getColor(const uint8_t color, const uint8_t slot, const bool setFadeDuration)
  {
    RgbLed led;

    if ((color <= static_cast<uint8_t>(Color::Color1)) && (slot <= static_cast<uint8_t>(Slot::SlotCalculated)))
    {
      led = _color[color][slot];

      if (setFadeDuration == true)
      {
        led.setDuration(_setting[static_cast<uint8_t>(FadeDuration)]);
      }
    }

    return led;
  }


  void Settings::setColors(const Slot slot, const RgbLed color0, const RgbLed color1)
  {
    setColors(static_cast<uint8_t>(slot), color0, color1);
  }


  void Settings::setColors(const uint8_t slot, const RgbLed color0, const RgbLed color1)
  {
    if ((slot <= static_cast<uint8_t>(Slot::SlotDmx)))
    {
      _color[Color::Color0][slot] = color0;
      _color[Color::Color1][slot] = color1;
    }
  }


  void Settings::refreshCalculatedColors()
  {
    DateTime currentTime = Hardware::getDateTime();
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
    // _color[Color::Color0][Slot::SlotCalculated].setDuration(_setting[static_cast<uint8_t>(FadeRate)]);
    // _color[Color::Color1][Slot::SlotCalculated].setDuration(_setting[static_cast<uint8_t>(FadeRate)]);

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

        _color[Color::Color0][Slot::SlotCalculated].setFromMergedRgbLeds(percentage, _color[Color::Color0][prevSlot], _color[Color::Color0][nextSlot]);
        _color[Color::Color1][Slot::SlotCalculated].setFromMergedRgbLeds(percentage, _color[Color::Color1][prevSlot], _color[Color::Color1][nextSlot]);
      }
      else
      {
        _color[Color::Color0][Slot::SlotCalculated] = _color[Color::Color0][prevSlot];
        _color[Color::Color1][Slot::SlotCalculated] = _color[Color::Color1][prevSlot];
      }
    }
    else
    {
      _color[Color::Color0][Slot::SlotCalculated] = Application::gray;
      _color[Color::Color1][Slot::SlotCalculated] = Application::white;
    }
  }


}
