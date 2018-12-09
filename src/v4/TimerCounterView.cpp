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


void TimerCounterView::enter()
{
  _pSettings = Application::getSettingsPtr();
}


void TimerCounterView::keyHandler(Keys::Key key)
{
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

    _fadeRate = _pSettings->getRawSetting(Settings::Setting::FadeRate);
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
      _fadeRate = 0;
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
      _fadeRate = 0;
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
}


void TimerCounterView::loop()
{
  DateTime currentTime;
  RgbLed   color[2];
  uint32_t displayBitMask;
  TimerMode currentTimerMode = static_cast<TimerMode>(Application::getViewMode());

  currentTime = Hardware::getDateTime();

  // determine what colors we need to use, then determine the bitmask for the display
  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    color[0] = _pSettings->getColor0(Settings::Slot::SlotDmx);
    color[1] = _pSettings->getColor1(Settings::Slot::SlotDmx);
  }
  else
  {
    color[0] = _pSettings->getColor0(Settings::Slot::SlotTimer);
    color[1] = _pSettings->getColor1(Settings::Slot::SlotTimer);
    color[0].setRate(_fadeRate);
    color[1].setRate(_fadeRate);
  }

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
          _timerValue = _pSettings->getRawSetting(Settings::Setting::TimerResetValue);
        }
        AlarmHandler::clearAlarm();   // in case of external control (DMX-512)
        Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
        break;
    }
  }

  // alarm handling
  if (_countUp == true)
  {
    if ((_timerValue == _pSettings->getRawSetting(Settings::Setting::TimerResetValue)) && (_alarmReady == true))
    {
      AlarmHandler::activateLatchingAlarm();
      _alarmReady = false;
      Application::setViewMode(static_cast<ViewMode>(TimerMode::TimerStop));
    }
    else if ((_timerValue != _pSettings->getRawSetting(Settings::Setting::TimerResetValue)) && (_alarmReady == false))
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

  if ((_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true) &&
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
  if (_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true)
  {
    displayBitMask = Hardware::uint32ToBcd(_timerValue);
  }
  else
  {
    displayBitMask = _timerValue;
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color[0], color[1], displayBitMask);

  DisplayManager::writeDisplay(bcDisp);
}


}
