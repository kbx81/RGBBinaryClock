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
#include "Hardware.h"
#include "Settings.h"
#include "TimerCounterView.h"


namespace kbxBinaryClock {


void TimerCounterView::enter()
{
  _settings = Application::getSettings();

  _timerRun = false;
  _countUp = true;
}


void TimerCounterView::keyHandler(Keys::Key key)
{
  if (key == Keys::Key::A)
  {
    _timerRun = true;
  }

  if (key == Keys::Key::B)
  {
    _timerRun = false;
  }

  if (key == Keys::Key::C)
  {
    _timerValue = 0;
  }

  if (key == Keys::Key::D)
  {
    if (_timerRun)
    {
      _countUp = false;
    }
    else
    {
      _timerValue--;
    }
  }

  if (key == Keys::Key::U)
  {
    if (_timerRun)
    {
      _countUp = true;
    }
    else
    {
      _timerValue++;
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void TimerCounterView::loop()
{
  DateTime currentTime;
  RgbLed color0, color1;
  uint32_t displayBitMask;

  currentTime = Hardware::getDateTime();

  // determine what colors we need to use, then determine the bitmask for the display
  color0 = _settings.getColor0(Settings::Slot::SlotTimer);
  color1 = _settings.getColor1(Settings::Slot::SlotTimer);
  color0.setRate(_settings.getRawSetting(Settings::Setting::FadeRate));
  color1.setRate(_settings.getRawSetting(Settings::Setting::FadeRate));

  if (_timerRun && _lastTime != currentTime.secondsSinceMidnight(false))
  {
    _lastTime = currentTime.secondsSinceMidnight(false);

    if (_countUp)
    {
      _timerValue++;
    }
    else
    {
      _timerValue--;
    }
  }

  // display timer in BCD if settings say so
  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD))
  {
    displayBitMask = Hardware::uint32ToBcd((uint16_t)_timerValue);
  }
  else
  {
    displayBitMask = _timerValue;
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color0, color1, displayBitMask);

  Hardware::writeDisplay(bcDisp);
}


}
