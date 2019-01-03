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
#include "Display.h"
#include "DisplayManager.h"
#include "Settings.h"
#include "TimeDateTempView.h"


namespace kbxBinaryClock {


// number of seconds in a day
//
const uint32_t TimeDateTempView::cSecondsInADay = 60 * 60 * 24;


TimeDateTempView::TimeDateTempView()
  : _lastSwitchTime(0),
    _currentTemperature(0),
    _currentTime(DateTime()),
    _mode(Application::OperatingMode::OperatingModeFixedDisplay)
{
}


void TimeDateTempView::enter()
{
  Settings *pSettings = Application::getSettingsPtr();

  _currentTime = Hardware::getDateTime();

  _mode = Application::getOperatingMode();

  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  DisplayManager::setStatusLedAutoRefreshing(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm));
}


bool TimeDateTempView::keyHandler(Keys::Key key)
{
  FixedDisplayItem currentDisplayItem = static_cast<FixedDisplayItem>(Application::getViewMode());
  bool tick = false;

  _lastSwitchTime = _currentTime.secondsSinceMidnight(false);

  if ((key == Keys::Key::A) && (currentDisplayItem != FixedDisplayItem::Time))
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Time));
    tick = true;
  }

  if ((key == Keys::Key::B) && (currentDisplayItem != FixedDisplayItem::Date))
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Date));
    tick = true;
  }

  if ((key == Keys::Key::C) && (currentDisplayItem != FixedDisplayItem::Temperature))
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Temperature));
    tick = true;
  }

  if ((key == Keys::Key::D) && (currentDisplayItem != FixedDisplayItem::TimeSeconds))
  {
    Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::TimeSeconds));
    tick = true;
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
    tick = true;
  }

  return tick;
}


void TimeDateTempView::loop()
{
  Display  bcDisp;
  FixedDisplayItem nextDisplayItem, currentDisplayItem = static_cast<FixedDisplayItem>(Application::getViewMode());
  RgbLed   statusLed;
  Settings *pSettings = Application::getSettingsPtr();
  Settings::Slot colorSlot = Settings::Slot::SlotDmx;
  uint32_t changeDisplayTime = 0, itemDisplayDuration = 0, pixelOffBitMask = 0;

  // update these only as needed -- keeps temperature from bouncing incessantly
  if (_currentTime != Hardware::getDateTime())
  {
    _currentTime = Hardware::getDateTime();
    _currentTemperature = Hardware::temperature(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayFahrenheit),
                            pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
  }
  // determine the display duration of the current display item as well as the next item to display
  if (_mode == Application::OperatingMode::OperatingModeToggleDisplay)
  {
    switch (currentDisplayItem)
    {
      case FixedDisplayItem::Date:
      itemDisplayDuration = pSettings->getRawSetting(Settings::Setting::DateDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Temperature;
      break;

      case FixedDisplayItem::Temperature:
      itemDisplayDuration = pSettings->getRawSetting(Settings::Setting::TemperatureDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Time;
      break;

      default:
      itemDisplayDuration = pSettings->getRawSetting(Settings::Setting::TimeDisplayDuration);
      nextDisplayItem = FixedDisplayItem::Date;
    }
    // determine when we need to switch the display
    changeDisplayTime = _lastSwitchTime + itemDisplayDuration;

    // rotate the display if it's time to do so
    if ((_currentTime.secondsSinceMidnight(false) >= changeDisplayTime) ||
        (_currentTime.secondsSinceMidnight(false) + cSecondsInADay == changeDisplayTime))
    {
      Application::setViewMode(static_cast<ViewMode>(nextDisplayItem));
      _lastSwitchTime = _currentTime.secondsSinceMidnight(false);
    }
  }

  // determine display colors
  if ((Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum)
      || (Hardware::rtcIsSet() == false))
  {
    switch (currentDisplayItem)
    {
      case FixedDisplayItem::Date:
      colorSlot = Settings::Slot::SlotDate;
      break;

      case FixedDisplayItem::Temperature:
      colorSlot = Settings::Slot::SlotTemperature;
      break;

      default:
      pSettings->refreshCalculatedColors();
      colorSlot = Settings::Slot::SlotCalculated;
    }
  }
  // set display colors and fade durations
  bcDisp.setDisplayColor0(pSettings->getColor(Settings::Color::Color0, colorSlot, Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum));
  bcDisp.setDisplayColor1(pSettings->getColor(Settings::Color::Color1, colorSlot, Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum));
  // set status LED to appropriate color if enabled as AM/PM indicator
  if (pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm) == true)
  {
    // this will set the color and (more importantly) fade duration correctly
    statusLed = pSettings->getColor(static_cast<Settings::Color>(_currentTime.isPM()), colorSlot, Application::getExternalControlState() != Application::ExternalControl::Dmx512ExtControlEnum);
    // turn it off if we're not displaying the time (a bit silly but it works)
    if ((currentDisplayItem != FixedDisplayItem::Time) && (currentDisplayItem != FixedDisplayItem::TimeSeconds))
    {
      statusLed.setOff();
    }
  }

  // set the display bits as appropriate for time, date, or temperature
  if ((Hardware::rtcIsSet() == true) || (currentDisplayItem == FixedDisplayItem::Temperature))
  {
    switch (currentDisplayItem)
    {
      case FixedDisplayItem::Date:
      bcDisp.setDisplayFromBytes(_currentTime.yearShort(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)),
                                 _currentTime.month(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)),
                                 _currentTime.day(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)));
      break;

      case FixedDisplayItem::Temperature:
      bcDisp.setDisplayFromBytes(0, _currentTemperature, 0);
      pixelOffBitMask = 0xff00ff;
      break;

      case FixedDisplayItem::TimeSeconds:
      bcDisp.setDisplayFromBitmap(_currentTime.secondsSinceMidnight(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)));
      break;

      default:
      bcDisp.setDisplayFromBytes(_currentTime.hour(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD),
                                   pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour)),
                                 _currentTime.minute(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)),
                                 _currentTime.second(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)));
    }
  }
  else
  {
    // make the LSbs blink if the clock is not set
    bcDisp.setDisplayFromBytes(_currentTime.second(false) & 1, _currentTime.second(false) & 1, _currentTime.second(false) & 1);
    // also make the status LED blink
    DisplayManager::setStatusLedAutoRefreshing(true);
    statusLed = bcDisp.getPixelRaw(0);
  }

  if (pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::MSBsOff) == true)
  {
    bcDisp.setMsbPixelsOff(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD), 0xfefefe);
  }
  bcDisp.setPixelsOff(pixelOffBitMask);

  DisplayManager::writeDisplay(bcDisp, statusLed);
}


}
