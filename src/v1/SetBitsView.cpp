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
#include "DisplayManager.h"
#include "Hardware.h"
#include "Settings.h"
#include "SetBitsView.h"


namespace kbxBinaryClock {

// Percentage of configured LED color intensities used for lowlight
//  (Percentages are times 100 -- e.g.: 2500 = 25.00%)
const uint16_t SetBitsView::cLowlightPercentage = 2500;


SetBitsView::SetBitsView()
  : _setBits(0),
    _bitsMask(0),
    _selectedBit(0),
    _mode(Application::OperatingMode::OperatingModeSetSystemOptions),
    _settings(Settings())
{
}


void SetBitsView::enter()
{
  _mode = Application::getOperatingMode();

  _settings = Application::getSettings();

  _setBits = _settings.getRawSetting(Application::getOperatingModeRelatedSetting(_mode));
  _bitsMask = Settings::cSettingData[Application::getOperatingModeRelatedSetting(_mode)];

  _selectedBit = 0;
}


bool SetBitsView::keyHandler(Keys::Key key)
{
  bool tick = true;

  if (key == Keys::Key::A)
  {
    _settings.setRawSetting(Application::getOperatingModeRelatedSetting(_mode), _setBits);
    Application::setSettings(_settings);

    DisplayManager::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    if ((_setBits & (1 << _selectedBit)) != 0)
    {
      _setBits &= ~(1 << _selectedBit);
    }
    else
    {
      tick = false;
    }
  }

  if (key == Keys::Key::C)
  {
    if ((_setBits & (1 << _selectedBit)) == 0)
    {
      _setBits |= (1 << _selectedBit);
    }
    else
    {
      tick = false;
    }
  }

  if (key == Keys::Key::D)
  {
    if (--_selectedBit > 15)
    {
      _selectedBit = 0;

      tick = false;
    }
  }

  if (key == Keys::Key::U)
  {
    if (++_selectedBit > __builtin_ctz(~_bitsMask) - 1)
    {
      _selectedBit = __builtin_ctz(~_bitsMask) - 1;

      tick = false;
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }

  return tick;
}


void SetBitsView::loop()
{
  RgbLed color0Highlight = _settings.getColor(Settings::Color::Color0, Settings::Slot::SlotSet),
         color1Highlight = _settings.getColor(Settings::Color::Color1, Settings::Slot::SlotSet),
         color0Lowlight = _settings.getColor(Settings::Color::Color0, Settings::Slot::SlotSet),
         color1Lowlight = _settings.getColor(Settings::Color::Color1, Settings::Slot::SlotSet);

  color0Lowlight.adjustIntensity(SetBitsView::cLowlightPercentage);
  color1Lowlight.adjustIntensity(SetBitsView::cLowlightPercentage);

  // create a new display object with the right colors and bitmask
  Display bcDisp(color0Lowlight, color1Lowlight, (uint32_t)_setBits);
  // highlight the selected region
  if ((_setBits >> _selectedBit) & 1)
  {
    bcDisp.setPixelFromRaw(_selectedBit, color1Highlight);
  }
  else
  {
    bcDisp.setPixelFromRaw(_selectedBit, color0Highlight);
  }

  DisplayManager::writeDisplay(bcDisp);
}


}
