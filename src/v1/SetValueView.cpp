//
// kbx81's binary clock set an arbitrary value view class
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
#include "DisplayManager.h"
#include "Hardware.h"
#include "Settings.h"
#include "SetValueView.h"


namespace kbxBinaryClock {


SetValueView::SetValueView()
  : _setValue(0),
    _maxValue(0),
    _mode(Application::OperatingMode::OperatingModeSetTimerResetValue),
    _settings(Settings())
{
}


void SetValueView::enter()
{
  _mode = Application::getOperatingMode();

  _settings = Application::getSettings();

  // Subtract the first in the OperatingModeSetDurationClock series from the
  //   current mode to get the value we're modifying
  _setValue = _settings.getRawSetting(Application::getOperatingModeRelatedSetting(_mode));
  _maxValue = Settings::cSettingData[Application::getOperatingModeRelatedSetting(_mode)];
}


bool SetValueView::keyHandler(Keys::Key key)
{
  bool tick = true;

  if (key == Keys::Key::A)
  {
    // Subtract the first in the OperatingModeSetDurationClock series from the
    //   current mode to get the value we're modifying
    _settings.setRawSetting(Application::getOperatingModeRelatedSetting(_mode), _setValue);
    Application::setSettings(_settings);

    DisplayManager::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    if (_setValue == 1)
    {
      tick = false;
    }
    else
    {
      _setValue = 1;
    }
  }

  if (key == Keys::Key::C)
  {
    if (_setValue == _maxValue)
    {
      tick = false;
    }
    else
    {
      _setValue = _maxValue;
    }
  }

  if (key == Keys::Key::D)
  {
    if (--_setValue > _maxValue)
    {
      _setValue = 0;
      tick = false;
    }

    if (!_setValue && (_maxValue == 12 || _maxValue == 4))  // it's a month or an ordinal
    {
      _setValue = 1;
      tick = false;
    }
  }

  if (key == Keys::Key::U)
  {
    if (++_setValue > _maxValue)
    {
      _setValue = _maxValue;
      tick = false;
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }

  return tick;
}


void SetValueView::loop()
{
  uint32_t displayBitMask, displayedValue = _setValue;

  // this is because DMX-512 starts counting at one and not at zero
  if (_mode == Application::OperatingMode::OperatingModeSetDMX512Address)
  {
    displayedValue += 1;
  }

  // display value in BCD if settings say so
  if (_settings.getSetting(Settings::SystemOptions, Settings::SystemOptionsBits::DisplayBCD))
  {
    displayBitMask = Hardware::uint32ToBcd(displayedValue);
  }
  else
  {
    displayBitMask = displayedValue;
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(_settings.getColor(Settings::Color::Color0, Settings::Slot::SlotSet), _settings.getColor(Settings::Color::Color1, Settings::Slot::SlotSet), displayBitMask);

  DisplayManager::writeDisplay(bcDisp);
}


}
