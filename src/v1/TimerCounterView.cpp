//
// kbx81's binary clock timer/counter view class
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

#include "AlarmHandler.h"
#include "Application.h"
#include "DisplayManager.h"
#include "Hardware.h"
#include "Settings.h"
#include "TimerCounterView.h"


namespace kbxBinaryClock {


// number of seconds in a day
//
const uint32_t TimerCounterView::cMaxBcdValue = 999999;


TimerCounterView::TimerCounterView()
  : _lastTime(0),
    _timerValue(0),
    _fadeDuration(0),
    _countUp(false),
    _alarmReady(false)
{
}


void TimerCounterView::enter()
{
  // we don't use the status LED, so turn it off in case it was left on
  DisplayManager::writeStatusLed(RgbLed());
}


bool TimerCounterView::keyHandler(Keys::Key key)
{
  Settings *pSettings = Application::getSettingsPtr();
  bool tick = true;

  if (key == Keys::Key::A)
  {
    if (_countUp == true)
    {
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerRunUp));
    }
    else
    {
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerRunDown));
    }

    _fadeDuration = pSettings->getRawSetting(Settings::Setting::FadeDuration);
  }

  if (key == Keys::Key::B)
  {
    Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
  }

  if (key == Keys::Key::C)
  {
    Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerReset));
  }

  if (key == Keys::Key::D)
  {
    if (static_cast<TimerMode>(Application::getViewMode()) == TimerMode::TimerStop)
    {
      _timerValue--;
      _fadeDuration = 0;
    }
    else
    {
      _countUp = false;
    }
  }

  if (key == Keys::Key::U)
  {
    if (static_cast<TimerMode>(Application::getViewMode()) == TimerMode::TimerStop)
    {
      _timerValue++;
      _fadeDuration = 0;
    }
    else
    {
      _countUp = true;
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }

  return tick;
}


void TimerCounterView::loop()
{
  DateTime  currentTime;
  Display   bcDisp;
  Settings *pSettings = Application::getSettingsPtr();
  TimerMode currentTimerMode = static_cast<TimerMode>(Application::getViewMode());

  currentTime = Hardware::getDateTime();

  // determine display colors
  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    bcDisp.setDisplayColor0(pSettings->getColor(Settings::Color::Color0, Settings::Slot::SlotDmx));
    bcDisp.setDisplayColor1(pSettings->getColor(Settings::Color::Color1, Settings::Slot::SlotDmx));
  }
  else
  {
    bcDisp.setDisplayColor0(pSettings->getColor(Settings::Color::Color0, Settings::Slot::SlotTimer, true));
    bcDisp.setDisplayColor1(pSettings->getColor(Settings::Color::Color1, Settings::Slot::SlotTimer, true));
  }
  // now determine the bitmask for the display
  if ((currentTimerMode != TimerMode::TimerStop) && (_lastTime != currentTime.secondsSinceMidnight(false)))
  {
    _lastTime = currentTime.secondsSinceMidnight(false);

    switch (currentTimerMode)
    {
      case TimerMode::TimerRunUp:
      _timerValue++;
      break;

      case TimerMode::TimerRunDown:
      _timerValue--;
      break;

      case TimerMode::TimerReset:
      if (_countUp == true)
      {
        _timerValue = 0;
      }
      else
      {
        _timerValue = pSettings->getRawSetting(Settings::Setting::TimerResetValue);
      }
      AlarmHandler::clearAlarm();   // in case of external control (DMX-512)
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
      break;

      default:
      break;
    }
  }

  // alarm handling
  if (_countUp == true)
  {
    if ((_timerValue == pSettings->getRawSetting(Settings::Setting::TimerResetValue)) && (_alarmReady == true))
    {
      AlarmHandler::activateLatchingAlarm();
      _alarmReady = false;
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
    }
    else if ((_timerValue != pSettings->getRawSetting(Settings::Setting::TimerResetValue)) && (_alarmReady == false))
    {
      _alarmReady = true;
    }
  }
  else
  {
    if ((_timerValue == 0) && (_alarmReady == true))
    {
      AlarmHandler::activateLatchingAlarm();
      _alarmReady = false;
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
    }
    else if ((_timerValue != 0) && (_alarmReady == false))
    {
      _alarmReady = true;
    }
  }

  if ((pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true) &&
      (_timerValue > cMaxBcdValue))
  {
    // we'll use what the timer value is closer to to determine what it
    //  should be reset to
    if (_timerValue < cMaxBcdValue * 8)
    {
      _timerValue = 0;
    }
    else
    {
      _timerValue = cMaxBcdValue;
    }
  }

  // display timer in BCD if settings say so
  if (pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true)
  {
    bcDisp.setDisplayFromBitmap(Hardware::uint32ToBcd(_timerValue));
  }
  else
  {
    bcDisp.setDisplayFromBitmap(_timerValue);
  }

  if (pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::MSBsOff) == true)
  {
    bcDisp.setMsbPixelsOff(pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));
  }

  DisplayManager::writeDisplay(bcDisp);
}


}
