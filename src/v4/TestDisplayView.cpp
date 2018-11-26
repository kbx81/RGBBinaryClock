//
// kbx81's binary clock TestDisplayView class
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
#include "Display.h"
#include "DisplayManager.h"
#include "Settings.h"
#include "TestDisplayView.h"


namespace kbxBinaryClock {


// maximum value for _displayedColor (2 = Blue)
//
const uint8_t TestDisplayView::cMaxColor = 2;


void TestDisplayView::enter()
{
  _mode = Application::getOperatingMode();

  _pSettings = Application::getSettingsPtr();

  DisplayManager::setStatusLedAutoRefreshing(true);

  // make the display bright so we can see the colors we're working with
  Application::setIntensityAutoAdjust(false);
  // set the display intensity
  DisplayManager::setMasterIntensity(_intensity);
  // Initialize the display drivers' dot control to the lowest level
  for (uint8_t i = 0; i < Display::cLedCount; i++)
  {
    DisplayManager::setDotCorrectionValue(i, 1);
  }
  Hardware::setLedCurrentDrive(0);
}


void TestDisplayView::keyHandler(Keys::Key key)
{
  if (key == Keys::Key::A)
  {
    _intensity = 0;
    // set the new display intensity
    DisplayManager::setMasterIntensity(_intensity);
  }

  if (key == Keys::Key::B)
  {
    _intensity = 10000;
    // set the new display intensity
    DisplayManager::setMasterIntensity(_intensity);
  }

  if (key == Keys::Key::C)
  {
    if (++_displayedColor > cMaxColor)
    {
      _displayedColor = 0;
    }
  }

  if (key == Keys::Key::D)
  {
    // set the new display intensity
    DisplayManager::setMasterIntensity(--_intensity);
  }

  if (key == Keys::Key::U)
  {
    // set the new display intensity
    DisplayManager::setMasterIntensity(++_intensity);
  }

  if (key == Keys::Key::E)
  {
    for (uint8_t i = 0; i < Display::cLedCount; i++)
    {
      DisplayManager::setDotCorrectionValue(i, 0x7f);
    }

    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void TestDisplayView::loop()
{
  const uint32_t displayBitMask = 0;
  const uint16_t testIntensity = 512;
  uint16_t red = 0, green = 0, blue = 0, rate = 0;

  switch (_displayedColor)
  {
    case 2:
      blue = testIntensity;
      break;

    case 1:
      green = testIntensity;
      break;

    default:
      red = testIntensity;
  }

  // now we can create a new display object with the right colors and bitmask
  RgbLed  testColor(red, green, blue, rate);
  Display bcDisp(testColor, testColor, displayBitMask);

  DisplayManager::writeDisplay(bcDisp);
  // Hardware::setStatusLed(testColor);
}


}
