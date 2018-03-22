//
// kbx81's binary clock set bits view class
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
#include "Hardware.h"
#include "Settings.h"
#include "SetBitsView.h"


namespace kbxBinaryClock {


void SetBitsView::enter()
{
  _mode = Application::getMode();

  _settings = Application::getSettings();

  _setBits = _settings.getRawSetting(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSetSystemOptions));
  _bitsMask = Settings::cSettingData[static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSetSystemOptions)];
}


void SetBitsView::keyHandler(Keys::Key key)
{
  if (key == Keys::Key::A)
  {
    _settings.setRawSetting(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSetSystemOptions), _setBits);
    Application::setSettings(_settings);

    Hardware::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    _setBits &= ~(1 << _selectedBit);
  }

  if (key == Keys::Key::C)
  {
    _setBits |= (1 << _selectedBit);
  }

  if (key == Keys::Key::D)
  {
    if (--_selectedBit > 15)
    {
      _selectedBit = 0;
    }
  }

  if (key == Keys::Key::U)
  {
    if (++_selectedBit >= __builtin_ctz(~_bitsMask) - 1)
    {
      _selectedBit = __builtin_ctz(~_bitsMask) - 1;
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void SetBitsView::loop()
{
  // create a new display object with the right colors and bitmask
  Display bcDisp(_settings.getColor0(Settings::Slot::SlotSetDim), _settings.getColor1(Settings::Slot::SlotSetDim), (uint32_t)_setBits);
  // highlight the selected region
  if ((_setBits >> _selectedBit) & 1)
  {
    bcDisp.setLedFromRaw(_selectedBit, _settings.getColor1(Settings::Slot::SlotSet));
  }
  else
  {
    bcDisp.setLedFromRaw(_selectedBit, _settings.getColor0(Settings::Slot::SlotSet));
  }

  Hardware::writeDisplay(bcDisp);
}


}
