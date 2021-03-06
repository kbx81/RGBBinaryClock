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


const uint32_t TimeDateTempView::cSecondsInADay = 60 * 60 * 24;

const uint8_t TimeDateTempView::cIntensityAdjustmentIncrement = 100;


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

  if (key == Keys::Key::A)
  {
    if (currentDisplayItem != FixedDisplayItem::Time)
    {
      Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::Time));
      tick = true;
    }
    else if (currentDisplayItem != FixedDisplayItem::TimeSeconds)
    {
      Application::setViewMode(static_cast<ViewMode>(FixedDisplayItem::TimeSeconds));
      tick = true;
    }
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

  if ((key == Keys::Key::D) && (Application::getIntensityAutoAdjust() == false))
  {
    const int16_t intensity = Application::getIntensity() - cIntensityAdjustmentIncrement;
    if (intensity > 0)
    {
      Application::setIntensity(intensity);
      tick = true;
    }
    else
    {
      Application::setIntensity(0);
    }
  }

  if ((key == Keys::Key::U) && (Application::getIntensityAutoAdjust() == false))
  {
    const int16_t intensity = Application::getIntensity() + cIntensityAdjustmentIncrement;
    if (intensity <= RgbLed::cLed100Percent)
    {
      Application::setIntensity(intensity);
      tick = true;
    }
    else
    {
      Application::setIntensity(RgbLed::cLed100Percent);
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
    tick = true;
  }

  if (tick == true)
  {
    _lastSwitchTime = _currentTime.secondsSinceMidnight(false);
  }

  return tick;
}


void TimeDateTempView::loop()
{
  Application::ExternalControl externalControlState = Application::getExternalControlState();
  Display  bcDisp;
  FixedDisplayItem nextDisplayItem, currentDisplayItem = static_cast<FixedDisplayItem>(Application::getViewMode());
  RgbLed   statusLed;
  Settings *pSettings = Application::getSettingsPtr();
  Settings::Slot colorSlot = Settings::Slot::SlotDmx;
  uint32_t changeDisplayTime = 0, itemDisplayDuration = 0, pixelOffBitMask = 0,
           secondsSinceMidnight = _currentTime.secondsSinceMidnight(false);
  bool displayBcd = pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD),
       display12Hour = pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour),
       displayFahrenheit = pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayFahrenheit);
  uint8_t year  = _currentTime.yearShort(displayBcd),
          month = _currentTime.month(displayBcd),
          day   = _currentTime.day(displayBcd);

  // update these only as needed -- keeps temperature from bouncing incessantly
  if (_currentTime != Hardware::getDateTime())
  {
    _currentTime = Hardware::getDateTime();
    _currentTemperature = Hardware::temperature(displayFahrenheit, displayBcd);
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
    if ((secondsSinceMidnight >= changeDisplayTime) || (secondsSinceMidnight + cSecondsInADay == changeDisplayTime))
    {
      Application::setViewMode(static_cast<ViewMode>(nextDisplayItem));
      _lastSwitchTime = secondsSinceMidnight;
    }
  }

  // determine display colors
  if ((externalControlState != Application::ExternalControl::Dmx512ExtControlEnum)
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
  bcDisp.setDisplayColor0(pSettings->getColor(Settings::Color::Color0, colorSlot, externalControlState != Application::ExternalControl::Dmx512ExtControlEnum));
  bcDisp.setDisplayColor1(pSettings->getColor(Settings::Color::Color1, colorSlot, externalControlState != Application::ExternalControl::Dmx512ExtControlEnum));
  // set status LED to appropriate color if enabled as AM/PM indicator
  if (pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StatusLedAsAmPm) == true)
  {
    // this will set the color and (more importantly) fade duration correctly
    statusLed = pSettings->getColor(static_cast<Settings::Color>(_currentTime.isPM()), colorSlot, externalControlState != Application::ExternalControl::Dmx512ExtControlEnum);
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
      switch (pSettings->getRawSetting(Settings::DateFormat))
      {
        case 2:
        bcDisp.setDisplayFromBytes(month, day, year);
        break;

        case 1:
        bcDisp.setDisplayFromBytes(day, month, year);
        break;

        default:
        bcDisp.setDisplayFromBytes(year, month, day);
      }
      break;

      case FixedDisplayItem::Temperature:
      bcDisp.setDisplayFromBytes(0, _currentTemperature, 0);
      pixelOffBitMask = 0xff00ff;
      break;

      case FixedDisplayItem::TimeSeconds:
      bcDisp.setDisplayFromBitmap(_currentTime.secondsSinceMidnight(displayBcd));
      break;

      default:
      bcDisp.setDisplayFromBytes(_currentTime.hour(displayBcd, display12Hour), _currentTime.minute(displayBcd), _currentTime.second(displayBcd));
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
    bcDisp.setMsbPixelsOff(displayBcd, 0xfefefe);
  }
  bcDisp.setPixelsOff(pixelOffBitMask);

  DisplayManager::writeDisplay(bcDisp, statusLed);
}


}
