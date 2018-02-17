//
// kbx81's binary clock MainMenuView class
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
#include "MainMenuView.h"
#include "Settings.h"


// Example of __DATE__ string: "Jul 27 2012"
//                              01234567890

#define BUILD_YEAR_CH0 (__DATE__[ 7] - 0x30)
#define BUILD_YEAR_CH1 (__DATE__[ 8] - 0x30)
#define BUILD_YEAR_CH2 (__DATE__[ 9] - 0x30)
#define BUILD_YEAR_CH3 (__DATE__[10] - 0x30)

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILD_MONTH_CH0 \
    ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? 1 : 0)

#define BUILD_MONTH_CH1 \
    ( \
        (BUILD_MONTH_IS_JAN) ? 1 : \
        (BUILD_MONTH_IS_FEB) ? 2 : \
        (BUILD_MONTH_IS_MAR) ? 3 : \
        (BUILD_MONTH_IS_APR) ? 4 : \
        (BUILD_MONTH_IS_MAY) ? 5 : \
        (BUILD_MONTH_IS_JUN) ? 6 : \
        (BUILD_MONTH_IS_JUL) ? 7 : \
        (BUILD_MONTH_IS_AUG) ? 8 : \
        (BUILD_MONTH_IS_SEP) ? 9 : \
        (BUILD_MONTH_IS_OCT) ? 0 : \
        (BUILD_MONTH_IS_NOV) ? 1 : \
        (BUILD_MONTH_IS_DEC) ? 2 : \
        /* error default */    '?' \
    )

#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4] - 0x30) : 0)
#define BUILD_DAY_CH1 (__DATE__[ 5] - 0x30)


// Example of __TIME__ string: "21:06:19"
//                              01234567

#define BUILD_HOUR_CH0 (__TIME__[0] - 0x30)
#define BUILD_HOUR_CH1 (__TIME__[1] - 0x30)

#define BUILD_MIN_CH0 (__TIME__[3] - 0x30)
#define BUILD_MIN_CH1 (__TIME__[4] - 0x30)

#define BUILD_SEC_CH0 (__TIME__[6] - 0x30)
#define BUILD_SEC_CH1 (__TIME__[7] - 0x30)


namespace kbxBinaryClock {


void MainMenuView::enter()
{
  _settings = Application::getSettings();

  if ((_selectedMode == 0) || (_selectedMode > Application::OperatingMode::OperatingModeTimerCounterColors))
  {
    _selectedMode = Application::OperatingMode::OperatingModeFixedDisplay;
  }
}


void MainMenuView::keyHandler(Keys::Key key)
{
  if (key == Keys::Key::A)
  {
    if (!Hardware::rtcIsSet())
    {
      DateTime today(2018, 2, 5, 1, 0, 0);
      Hardware::setDateTime(today);
      // Hardware::setTimeBCD((BUILD_HOUR_CH0 << 4) | BUILD_HOUR_CH1, (BUILD_MIN_CH0 << 4) | BUILD_MIN_CH1, (BUILD_SEC_CH0 << 4) | BUILD_SEC_CH1, true, false);
      // Hardware::setDateBCD((BUILD_YEAR_CH2 << 4) | BUILD_YEAR_CH3, (BUILD_MONTH_CH0 << 4) | BUILD_MONTH_CH1, (BUILD_DAY_CH0 << 4) | BUILD_DAY_CH1);
      Hardware::doubleBlink();
    }
    else
    {
      if (_selectedMode == static_cast<uint8_t>(Application::OperatingMode::OperatingModeTimerCounterColors) + 1)
      {
        Hardware::redLed(4095);

        Hardware::eraseFlash(0x0801f000);
        _selectedMode = 1;

        Hardware::doubleBlink();
        Hardware::doubleBlink();
        Hardware::redLed(0);
      }
    }
  }

  if (key == Keys::Key::B)
  {
    Application::setMode(Application::OperatingMode::OperatingModeFixedDisplay);
  }

  if (key == Keys::Key::C)
  {
    Application::setMode(Application::OperatingMode::OperatingModeToggleDisplay);
  }

  if (key == Keys::Key::D)
  {
    if (--_selectedMode == 0)
    {
      _selectedMode = 1;
    }
  }

  if (key == Keys::Key::U)
  {
    _selectedMode++;

    if (_selectedMode > static_cast<uint8_t>(Application::OperatingMode::OperatingModeTimerCounterColors) + 1)
    {
      _selectedMode = static_cast<uint8_t>(Application::OperatingMode::OperatingModeTimerCounterColors) + 1;
    }
  }

  if (key == Keys::Key::E)
  {
    if (_selectedMode <= static_cast<uint8_t>(Application::OperatingMode::OperatingModeTimerCounterColors))
    {
      Application::setMode((Application::OperatingMode)_selectedMode);
    }
  }
}


void MainMenuView::loop()
{
  uint16_t rate = 0;
  uint32_t displayBitMask = Application::getModeDisplayNumber(_selectedMode);
  RgbLed  menuWhite(4095, 4095, 4095, rate),
          menuDarkWhite(512, 512, 512, rate);

  if (displayBitMask == 0)
  {
    // fix up the last "Erase FLASH" mode value
    displayBitMask = 99;
  }

  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD))
  {
    displayBitMask = Hardware::uint32ToBcd(static_cast<uint16_t>(displayBitMask));
  }

  Display bcDisp(menuDarkWhite, menuWhite, displayBitMask << 16);

  Hardware::writeDisplay(bcDisp);
}


}
