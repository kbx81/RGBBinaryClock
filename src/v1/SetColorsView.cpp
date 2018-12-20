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
#include "DisplayManager.h"
#include "Hardware.h"
#include "RgbLed.h"
#include "Settings.h"
#include "SetColorsView.h"


namespace kbxBinaryClock {


const uint16_t SetColorsView::cMaxDelta = 256;
const uint8_t SetColorsView::cDeltaShiftAmt = 4;  // shift (left) by eight bits


void SetColorsView::enter()
{
  _mode = Application::getOperatingMode();

  _settings = Application::getSettings();

  _delta = cMaxDelta;
  _selectedParam = Color::Red0;

  // Subtract the first in the OperatingModeSlotxColors series from the
  //   current mode to get the slot we're modifying
  RgbLed color0(_settings.getColor0(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors))),
         color1(_settings.getColor1(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors)));

  _setValues[Color::Red0] = color0.getRed();
  _setValues[Color::Green0] = color0.getGreen();
  _setValues[Color::Blue0] = color0.getBlue();
  _setValues[Color::Red1] = color1.getRed();
  _setValues[Color::Green1] = color1.getGreen();
  _setValues[Color::Blue1] = color1.getBlue();

  // make the display bright so we can see the colors we're working with
  Application::setIntensityAutoAdjust(false);
}


void SetColorsView::keyHandler(Keys::Key key)
{
  SCVDisplayItem currentDisplayMode = static_cast<SCVDisplayItem>(Application::getViewMode());

  if (key == Keys::Key::A)
  {
    Application::setSettings(_settings);
    Application::setIntensityAutoAdjust(false);   // keep this disabled for now

    DisplayManager::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    if (currentDisplayMode == SCVDisplayItem::DeltaValue)
    {
      _delta = _delta << cDeltaShiftAmt;

      if (_delta > cMaxDelta)
      {
        _delta = 1;
      }
    }
    else
    {
      Application::setViewMode(static_cast<ViewMode>(SCVDisplayItem::DeltaValue));
    }
  }

  if (key == Keys::Key::C)
  {
    if (currentDisplayMode == SCVDisplayItem::ColorMixer)
    {
      if (++_selectedParam > Color::Blue1)
      {
        _selectedParam = Color::Red0;
      }
    }
    else
    {
      Application::setViewMode(static_cast<ViewMode>(SCVDisplayItem::ColorMixer));
    }
  }

  if (key == Keys::Key::D)
  {
    if (currentDisplayMode == SCVDisplayItem::ColorMixer)
    {
      _setValues[_selectedParam] -= _delta;
      _setValues[_selectedParam] &= 0xfff;
    }
    else
    {
      Application::setViewMode(static_cast<ViewMode>(SCVDisplayItem::ColorMixer));
    }
  }

  if (key == Keys::Key::U)
  {
    if (currentDisplayMode == SCVDisplayItem::ColorMixer)
    {
      _setValues[_selectedParam] += _delta;
      _setValues[_selectedParam] &= 0xfff;
    }
    else
    {
      Application::setViewMode(static_cast<ViewMode>(SCVDisplayItem::ColorMixer));
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void SetColorsView::loop()
{
  DateTime current = Hardware::getDateTime();
  uint32_t secondsBitMask = current.second(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD)),
           displayBitMask = _setValues[_selectedParam];
  uint16_t rate = 0;
  SCVDisplayItem currentDisplayMode = static_cast<SCVDisplayItem>(Application::getViewMode());
  RgbLed mixedColor0(_setValues[Color::Red0], _setValues[Color::Green0], _setValues[Color::Blue0], rate),
         mixedColor1(_setValues[Color::Red1], _setValues[Color::Green1], _setValues[Color::Blue1], rate),
         red(2048, 0, 0, rate),
         green(0, 2048, 0, rate),
         blue(0, 0, 2048, rate),
         white(2048, 2048, 2048, rate),
         gray(384, 384, 384, rate),
         off(0, 0, 0, rate);
  // bcDisp is initialized for DeltaValue view mode
  Display bcDisp(off, white, _delta << 8);
  // ...we change it here if we're not in that mode
  if (currentDisplayMode == SCVDisplayItem::ColorMixer)
  {
    // display value in BCD if settings say so
    if (_settings.getSetting(Settings::SystemOptions, Settings::SystemOptionsBits::DisplayBCD))
    {
     displayBitMask = Hardware::uint32ToBcd(_setValues[_selectedParam]);
    }
    // fix up bcDisp display object with the right colors and bitmask
    switch (_selectedParam)
    {
      case Color::Blue0:
        bcDisp.setDisplayColor0(blue);
        bcDisp.setDisplayColor1(white);
        break;

      case Color::Green0:
        bcDisp.setDisplayColor0(green);
        bcDisp.setDisplayColor1(white);
        break;

      case Color::Red0:
        bcDisp.setDisplayColor0(red);
        bcDisp.setDisplayColor1(white);
        break;

      case Color::Blue1:
        bcDisp.setDisplayColor0(gray);
        bcDisp.setDisplayColor1(blue);
        break;

      case Color::Green1:
        bcDisp.setDisplayColor0(gray);
        bcDisp.setDisplayColor1(green);
        break;

      case Color::Red1:
      default:
        bcDisp.setDisplayColor0(gray);
        bcDisp.setDisplayColor1(red);
        // break;
    }

    bcDisp.setDisplayFromBitmap(displayBitMask << 8);
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    if (((secondsBitMask >> i) & 1) == true)
    {
      bcDisp.setPixelFromRaw(i, mixedColor1);
    }
    else
    {
      bcDisp.setPixelFromRaw(i, mixedColor0);
    }
  }
  // Subtract the first in the OperatingModeSlotxColors series from the
  //   current mode to get the slot we're modifying
  _settings.setColors(static_cast<uint8_t>(_mode - Application::OperatingMode::OperatingModeSlot1Colors), mixedColor0, mixedColor1);

  DisplayManager::writeDisplay(bcDisp);
}


}
