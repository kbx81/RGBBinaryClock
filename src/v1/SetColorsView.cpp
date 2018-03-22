//
// kbx81's binary clock set colors view class
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
#include "Hardware.h"
#include "RgbLed.h"
#include "Settings.h"
#include "SetColorsView.h"


namespace kbxBinaryClock {


void SetColorsView::enter()
{
  RgbLed color0, color1;

  _delta = 0x80;

  _mode = Application::getMode();

  _settings = Application::getSettings();

  // Subtract the first in the OperatingModeSlotxColors series from the
  //   current mode to get the slot we're modifying
  color0 = _settings.getColor0(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors));
  color1 = _settings.getColor1(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors));

  _setValues[Color::Red0] = color0.getRed();
  _setValues[Color::Green0] = color0.getGreen();
  _setValues[Color::Blue0] = color0.getBlue();
  _setValues[Color::Red1] = color1.getRed();
  _setValues[Color::Green1] = color1.getGreen();
  _setValues[Color::Blue1] = color1.getBlue();

  // make the display bright so we can see the colors we're working with
  Hardware::autoAdjustIntensities(false);
}


void SetColorsView::keyHandler(Keys::Key key)
{
  RgbLed color0(_setValues[Color::Red0], _setValues[Color::Green0], _setValues[Color::Blue0], 0),
         color1(_setValues[Color::Red1], _setValues[Color::Green1], _setValues[Color::Blue1], 0);

  if (key == Keys::Key::A)
  {
    // Subtract the first in the OperatingModeSlotxColors series from the
    //   current mode to get the slot we're modifying
    _settings.setColors(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors), color0, color1);

    Application::setSettings(_settings);
    Hardware::autoAdjustIntensities(false);   // keep this disabled for now

    Hardware::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    _delta += 0x80;
    _delta &= 0x7ff;
    _delta |= 1;
  }

  if (key == Keys::Key::C)
  {
    if (++_selectedParam > Color::Blue1)
    {
      _selectedParam = Color::Red0;
    }
  }

  if (key == Keys::Key::D)
  {
    _setValues[_selectedParam] -= _delta;
    _setValues[_selectedParam] &= 0xfff;
  }

  if (key == Keys::Key::U)
  {
    _setValues[_selectedParam] += _delta;
    _setValues[_selectedParam] &= 0xfff;
  }

  if (key == Keys::Key::E)
  {
    // restore autoAdjustIntensities in case we changed it
    Hardware::autoAdjustIntensities(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));

    Application::setMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void SetColorsView::loop()
{
  uint16_t rate = 0;
  uint32_t displayBitMask;
  DateTime current = Hardware::getDateTime();
  RgbLed color0(_setValues[Color::Red0], _setValues[Color::Green0], _setValues[Color::Blue0], 0),
         color1(_setValues[Color::Red1], _setValues[Color::Green1], _setValues[Color::Blue1], 0),
         redHighlight(4095, 0, 0, rate),
         greenHighlight(0, 4095, 0, rate),
         blueHighlight(0, 0, 4095, rate),
         redLowlight(512, 0, 0, rate),
         greenLowlight(0, 512, 0, rate),
         blueLowlight(0, 0, 512, rate),
         white(4095, 4095, 4095, rate),
         off(0, 0, 0, rate);

  // display value in BCD if settings say so
  displayBitMask = current.second(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD));

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color0, color1, displayBitMask);

  bcDisp.setLedFromRaw( 8, off);
  bcDisp.setLedFromRaw( 9, off);
  bcDisp.setLedFromRaw(10, off);
  bcDisp.setLedFromRaw(11, off);

  bcDisp.setLedFromRaw(16, redLowlight);
  bcDisp.setLedFromRaw(17, greenLowlight);
  bcDisp.setLedFromRaw(18, blueLowlight);
  bcDisp.setLedFromRaw(19, off);
  bcDisp.setLedFromRaw(20, redLowlight);
  bcDisp.setLedFromRaw(21, greenLowlight);
  bcDisp.setLedFromRaw(22, blueLowlight);
  bcDisp.setLedFromRaw(23, off);

  for (uint8_t i = 12; i < 16; i++)
  {
    if ((_delta >> (i - 5)) & 1)
    {
      bcDisp.setLedFromRaw(i, white);
    }
    else
    {
      bcDisp.setLedFromRaw(i, off);
    }
  }

  switch (_selectedParam)
  {
    case Red0:
    bcDisp.setLedFromRaw(16, redHighlight);
    break;

    case Green0:
    bcDisp.setLedFromRaw(17, greenHighlight);
    break;

    case Blue0:
    bcDisp.setLedFromRaw(18, blueHighlight);
    break;

    case Red1:
    bcDisp.setLedFromRaw(20, redHighlight);
    break;

    case Green1:
    bcDisp.setLedFromRaw(21, greenHighlight);
    break;

    case Blue1:
    bcDisp.setLedFromRaw(22, blueHighlight);
    break;
  }

  Hardware::writeDisplay(bcDisp);
}


}
