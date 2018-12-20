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
#include "DisplayManager.h"
#include "MainMenuView.h"
#include "Settings.h"


namespace kbxBinaryClock {


void MainMenuView::enter()
{
  _settings = Application::getSettings();

  // correct the mode if it's out of range
  if ((_selectedMode == 0) || (_selectedMode > Application::OperatingMode::OperatingModeSetColors))
  {
    _selectedMode = Application::OperatingMode::OperatingModeFixedDisplay;
  }
  // if the clock is not set, select the SetClock mode automagically
  if (Hardware::rtcIsSet() == false)
  {
    _selectedMode = Application::OperatingMode::OperatingModeSetClock;
  }
  // if the clock is set but not the date, select the SetDate mode automagically
  else if (Hardware::getDateTime().yearShort(false) == 0)
  {
    _selectedMode = Application::OperatingMode::OperatingModeSetDate;
  }

  // Verify/restore hardware settings
  Hardware::setVolume(_settings.getRawSetting(Settings::Setting::BeeperVolume));
  Hardware::setLedCurrentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Application::setIntensityAutoAdjust(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));
  DisplayManager::setStatusLedAutoRefreshing(false);
  DisplayManager::setDisplayBlanking(false);
  // Make sure the status LEDs are off
  Hardware::setBlueLed(0);
  Hardware::setGreenLed(0);
  Hardware::setRedLed(0);
}


void MainMenuView::keyHandler(Keys::Key key)
{
  if ((key == Keys::Key::A) &&
      (_selectedMode == static_cast<uint8_t>(Application::OperatingMode::OperatingModeTestDisplay) + 1))
  {
    Hardware::setRedLed(4095);

    Hardware::eraseFlash(0x0801f000);
    _selectedMode = 1;

    DisplayManager::doubleBlink();
    DisplayManager::doubleBlink();
    Hardware::setRedLed(0);
  }

  if (key == Keys::Key::B)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeFixedDisplay);
  }

  if (key == Keys::Key::C)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeToggleDisplay);
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

    if (_selectedMode > static_cast<uint8_t>(Application::OperatingMode::OperatingModeTestDisplay) + 1)
    {
      _selectedMode = static_cast<uint8_t>(Application::OperatingMode::OperatingModeTestDisplay) + 1;
    }
  }

  if (key == Keys::Key::E)
  {
    if (_selectedMode <= static_cast<uint8_t>(Application::OperatingMode::OperatingModeTestDisplay))
    {
      Application::setOperatingMode((Application::OperatingMode)_selectedMode);
    }
  }
}


void MainMenuView::loop()
{
  uint32_t displayBitMask = Application::getModeDisplayNumber(_selectedMode);

  if (displayBitMask == 0)
  {
    // fix up the last "Erase FLASH" mode value
    displayBitMask = 99;
  }

  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD))
  {
    displayBitMask = Hardware::uint32ToBcd(static_cast<uint16_t>(displayBitMask));
  }

  Display bcDisp(_settings.getColor0(Settings::Slot::SlotMenu), _settings.getColor1(Settings::Slot::SlotMenu), displayBitMask << 16);

  DisplayManager::writeDisplay(bcDisp);
}


}
