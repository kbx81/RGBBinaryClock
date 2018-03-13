//
// kbx81's binary clock TimeDateTempView class
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
#include "Settings.h"
#include "TimeDateTempView.h"


namespace kbxBinaryClock {


// number of seconds in a day
//
const uint32_t TimeDateTempView::SecondsInADay = 60 * 60 * 24;


bool TimeDateTempView::_isDst()
{
  uint8_t firstDay;

  _currentTime = Hardware::getDateTime();

  if (_dstStart.year(false) != _currentTime.year(false))
  {
    _dstState = DstState::Reset;
    firstDay = ((_settings.getRawSetting(Settings::Setting::DstBeginDowOrdinal) - 1) * 7) + 1;

    do
    {
      _dstStart.setDate(_currentTime.year(false), _settings.getRawSetting(Settings::Setting::DstBeginMonth), firstDay++);
    }
    while((_dstStart.dayOfWeek() != _settings.getRawSetting(Settings::Setting::DstSwitchDayOfWeek)) && (firstDay <= 31));

    _dstStart.setTime(_settings.getRawSetting(Settings::Setting::DstSwitchHour), 0, 0);

    if (_currentTime >= _dstStart)
    {
      _dstState = DstState::Spring;
    }
  }

  if (_dstEnd.year(false) != _currentTime.year(false))
  {
    firstDay = ((_settings.getRawSetting(Settings::Setting::DstEndDowOrdinal) - 1) * 7) + 1;

    do
    {
      _dstEnd.setDate(_currentTime.year(false), _settings.getRawSetting(Settings::Setting::DstEndMonth), firstDay++);
    }
    while((_dstEnd.dayOfWeek() != _settings.getRawSetting(Settings::Setting::DstSwitchDayOfWeek)) && (firstDay <= 31));

    _dstEnd.setTime(_settings.getRawSetting(Settings::Setting::DstSwitchHour), 0, 0);

    if (_currentTime >= _dstEnd)
    {
      _dstState = DstState::Fall;
    }
  }

  // once everything is calculated above, it becomes this simple...
  return (_currentTime >= _dstStart && _currentTime < _dstEnd);
}


void TimeDateTempView::enter()
{
  _currentTime = Hardware::getDateTime();

  _fixedDisplayItem = FixedDisplayItem::Time;

  _mode = Application::getMode();

  _settings = Application::getSettings();

  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  Hardware::autoRefreshStatusLed(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm));
}


void TimeDateTempView::keyHandler(Keys::Key key)
{
  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  if (key == Keys::Key::A)
  {
    _fixedDisplayItem = FixedDisplayItem::Time;
  }

  if (key == Keys::Key::B)
  {
    _fixedDisplayItem = FixedDisplayItem::Date;
  }

  if (key == Keys::Key::C)
  {
    _fixedDisplayItem = FixedDisplayItem::Temperature;
  }

  if (key == Keys::Key::D)
  {
    _fixedDisplayItem = FixedDisplayItem::TimeSeconds;
  }

  if (key == Keys::Key::E)
  {
    // Make sure these are off before we leave
    Hardware::autoRefreshStatusLed(false);
    Hardware::blueLed(0);
    Hardware::greenLed(0);
    Hardware::redLed(0);

    Application::setMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void TimeDateTempView::loop()
{
  FixedDisplayItem nextDisplayItem;
  RgbLed   color[2], defaultOff(0, 0, 0, _settings.getRawSetting(Settings::Setting::FadeRate));
  uint32_t displayBitMask, changeDisplayTime;

  _currentTime = Hardware::getDateTime();

  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DstEnable))
  {
    if ((_currentTime >= _dstEnd) && (_dstState == DstState::Spring))
    {
      _dstState = DstState::Fall;
      Hardware::adjustForDst(_isDst());
    }

    if ((_currentTime >= _dstStart) && (_dstState == DstState::Reset))
    {
      _dstState = DstState::Spring;
      Hardware::adjustForDst(_isDst());
    }
  }

  if (_mode == Application::OperatingMode::OperatingModeToggleDisplay)
  {
    switch (_fixedDisplayItem)
    {
      case FixedDisplayItem::Date:
      changeDisplayTime = _lastSwitchTime + _settings.getRawSetting(Settings::Setting::DateDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Temperature;
      break;

      case FixedDisplayItem::Temperature:
      changeDisplayTime = _lastSwitchTime + _settings.getRawSetting(Settings::Setting::TemperatureDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Time;
      break;

      default:
      changeDisplayTime = _lastSwitchTime + _settings.getRawSetting(Settings::Setting::TimeDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Date;
      break;
    }

    // rotate the display if it's time to do so
    if (((changeDisplayTime >= SecondsInADay) && (_currentTime.secondsSinceMidnight(false) + SecondsInADay >= changeDisplayTime)) ||
        (_currentTime.secondsSinceMidnight(false) >= changeDisplayTime))
    {
      _fixedDisplayItem = nextDisplayItem;
      _lastSwitchTime = _currentTime.secondsSinceMidnight(false);
    }
  }

  // Refresh display colors
  _settings.refreshCalculatedColors();
  color[0] = _settings.getColor0(Settings::Slot::SlotCalculated);
  color[1] = _settings.getColor1(Settings::Slot::SlotCalculated);

  // make the LSbs blink if the clock is not set
  if ((Hardware::rtcIsSet() == false) && (_fixedDisplayItem != FixedDisplayItem::Temperature))
  {
    displayBitMask = ((_currentTime.second(false) << 16) | (_currentTime.second(false) << 8) | _currentTime.second(false)) & 0x010101;
    // also make the status LED blink
    Hardware::autoRefreshStatusLed(true);
    Hardware::setStatusLed(color[_currentTime.second(false) & 1]);
  }
  else
  {
    switch (_fixedDisplayItem)
    {
      case FixedDisplayItem::TimeSeconds:
      displayBitMask = _currentTime.secondsSinceMidnight(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      break;

      case FixedDisplayItem::Date:
      color[0] = _settings.getColor0(Settings::Slot::SlotDate);
      color[1] = _settings.getColor1(Settings::Slot::SlotDate);

      displayBitMask = (_currentTime.yearShort(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 16)
                     | (_currentTime.month(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 8)
                     | _currentTime.day(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      break;

      case FixedDisplayItem::Temperature:
      color[0] = _settings.getColor0(Settings::Slot::SlotTemperature);
      color[1] = _settings.getColor1(Settings::Slot::SlotTemperature);

      displayBitMask = Hardware::temperature(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayFahrenheit),
                        _settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 8;
      break;

      default:
      displayBitMask = (_currentTime.hour(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD),
                        _settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour)) << 16)
                     | (_currentTime.minute(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 8)
                     | _currentTime.second(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      // break;
    }

    color[0].setRate(_settings.getRawSetting(Settings::Setting::FadeRate));
    color[1].setRate(_settings.getRawSetting(Settings::Setting::FadeRate));

    if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm))
    {
      if ((_fixedDisplayItem == FixedDisplayItem::Time) || (_fixedDisplayItem == FixedDisplayItem::TimeSeconds))
      {
        if (_currentTime.isPM())
        {
          Hardware::setStatusLed(color[1]);
        }
        else
        {
          Hardware::setStatusLed(color[0]);
        }
      }
      else
      {
        Hardware::setStatusLed(defaultOff);
      }
    }
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color[0], color[1], displayBitMask);

  Hardware::writeDisplay(bcDisp);
}


}
