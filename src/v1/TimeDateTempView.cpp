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

#include "Application.h"
#include "DateTime.h"
#include "DisplayManager.h"
#include "Settings.h"
#include "TimeDateTempView.h"


namespace kbxBinaryClock {


// number of seconds in a day
//
const uint32_t TimeDateTempView::cSecondsInADay = 60 * 60 * 24;


void TimeDateTempView::enter()
{
  _currentTime = Hardware::getDateTime();

  _mode = Application::getOperatingMode();

  _pSettings = Application::getSettingsPtr();

  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  DisplayManager::setStatusLedAutoRefreshing(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm));
}


void TimeDateTempView::keyHandler(Keys::Key key)
{
  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  if (key == Keys::Key::A)
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Time));
  }

  if (key == Keys::Key::B)
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Date));
  }

  if (key == Keys::Key::C)
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Temperature));
  }

  if (key == Keys::Key::D)
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::TimeSeconds));
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void TimeDateTempView::loop()
{
  FixedDisplayItem nextDisplayItem, currentDisplayItem = static_cast<FixedDisplayItem>(Application::getViewMode());
  RgbLed   color[2], statusLed(0, 0, 0, _pSettings->getRawSetting(Settings::Setting::FadeRate));
  uint32_t displayBitMask, changeDisplayTime, itemDisplayDuration, pixelOffBitMask = 0;

  // update these only as needed -- keeps temperature from bouncing incessantly
  if (_currentTime != Hardware::getDateTime())
  {
    _currentTime = Hardware::getDateTime();
    _currentTemperature = Hardware::temperature(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayFahrenheit),
                            _pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
  }

  if (_mode == Application::OperatingMode::OperatingModeToggleDisplay)
  {
    switch (currentDisplayItem)
    {
      case FixedDisplayItem::Date:
      itemDisplayDuration = _pSettings->getRawSetting(Settings::Setting::DateDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Temperature;
      break;

      case FixedDisplayItem::Temperature:
      itemDisplayDuration = _pSettings->getRawSetting(Settings::Setting::TemperatureDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Time;
      break;

      default:
      itemDisplayDuration = _pSettings->getRawSetting(Settings::Setting::TimeDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Date;
      break;
    }

    changeDisplayTime = _lastSwitchTime + itemDisplayDuration;

    // rotate the display if it's time to do so
    if ((_currentTime.secondsSinceMidnight(false) >= changeDisplayTime) ||
        (_currentTime.secondsSinceMidnight(false) + cSecondsInADay == changeDisplayTime))
    {
      Application::setViewMode(static_cast<ViewMode>(nextDisplayItem));
      _lastSwitchTime = _currentTime.secondsSinceMidnight(false);
    }
  }

  // Determine and/or refresh display colors
  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    color[0] = _pSettings->getColor0(Settings::Slot::SlotDmx);
    color[1] = _pSettings->getColor1(Settings::Slot::SlotDmx);
  }
  else
  {
    _pSettings->refreshCalculatedColors();
    color[0] = _pSettings->getColor0(Settings::Slot::SlotCalculated);
    color[1] = _pSettings->getColor1(Settings::Slot::SlotCalculated);
  }

  // make the LSbs blink if the clock is not set
  if ((Hardware::rtcIsSet() == false) && (currentDisplayItem != FixedDisplayItem::Temperature))
  {
    displayBitMask = ((_currentTime.second(false) << 16) | (_currentTime.second(false) << 8) | _currentTime.second(false)) & 0x010101;
    // also make the status LED blink
    DisplayManager::setStatusLedAutoRefreshing(true);
    Hardware::setStatusLed(color[_currentTime.second(false) & 1]);
  }
  else
  {
    switch (currentDisplayItem)
    {
      case FixedDisplayItem::TimeSeconds:
      displayBitMask = _currentTime.secondsSinceMidnight(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      break;

      case FixedDisplayItem::Date:
      if (Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum)
      {
        color[0] = _pSettings->getColor0(Settings::Slot::SlotDate);
        color[1] = _pSettings->getColor1(Settings::Slot::SlotDate);
      }

      displayBitMask = (_currentTime.yearShort(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 16)
                     | (_currentTime.month(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 8)
                     | _currentTime.day(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      break;

      case FixedDisplayItem::Temperature:
      if (Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum)
      {
        color[0] = _pSettings->getColor0(Settings::Slot::SlotTemperature);
        color[1] = _pSettings->getColor1(Settings::Slot::SlotTemperature);
      }

      displayBitMask = (_currentTemperature << 8);
      pixelOffBitMask = 0xff00ff;
      break;

      default:
      displayBitMask = (_currentTime.hour(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD),
                        _pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour)) << 16)
                     | (_currentTime.minute(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)) << 8)
                     | _currentTime.second(_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
      // break;
    }

    if (Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum)
    {
      color[0].setDuration(_pSettings->getRawSetting(Settings::Setting::FadeRate));
      color[1].setDuration(_pSettings->getRawSetting(Settings::Setting::FadeRate));
    }

    if (_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm))
    {
      if ((currentDisplayItem == FixedDisplayItem::Time) || (currentDisplayItem == FixedDisplayItem::TimeSeconds))
      {
        if (_currentTime.isPM())
        {
          statusLed = color[1];
        }
        else
        {
          statusLed = color[0];
        }
      }
    }
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color[0], color[1], displayBitMask);
  bcDisp.setPixelsOff(pixelOffBitMask);

  DisplayManager::writeDisplay(bcDisp, statusLed);
}


}
