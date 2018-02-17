//
// kbx81's binary clock main application
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
#include <cstdint>

#include "Application.h"
#include "AlarmHandler.h"
#include "Keys.h"
#include "Settings.h"
#include "MainMenuView.h"
#include "TimeDateTempView.h"
#include "TimerCounterView.h"
#include "SetTimeDateView.h"
#include "SetValueView.h"
#include "SetBitsView.h"
#include "SetColorsView.h"


// Workaround for  "hidden symbol `__dso_handle' isn't defined" linker error
void* __dso_handle;


namespace kbxBinaryClock {

namespace Application {


// An array with all views for the application
//
static View* const cModeViews[] = {
    new MainMenuView(),
    new TimeDateTempView(),
    new TimerCounterView(),
    new SetTimeDateView(),
    new SetBitsView(),
    new SetValueView(),
    new SetColorsView()
};

// Structure defining menu views
//
struct viewDescriptor {
  uint8_t  displayNumber;
  ViewEnum view;
};


// An array of mode display numbers and their corresponding views
//
static viewDescriptor const cViewDescriptor[] = {
    {  0, ViewEnum::MainMenuViewEnum },     // OperatingModeMainMenu
    {  1, ViewEnum::TimeDateTempViewEnum }, // OperatingModeFixedDisplay
    {  2, ViewEnum::TimeDateTempViewEnum }, // OperatingModeToggleDisplay
    {  3, ViewEnum::TimerCounterViewEnum }, // OperatingModeTimerCounter
    {  5, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSetClock
    {  6, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSetDate
    { 10, ViewEnum::SetBitsViewEnum },      // OperatingModeSetSystemOptions
    { 11, ViewEnum::SetBitsViewEnum },      // OperatingModeSlotBeepConfig
    { 12, ViewEnum::SetBitsViewEnum },      // OperatingModeSlotBlinkConfig
    { 13, ViewEnum::SetBitsViewEnum },      // OperatingModeSlotColorConfig
    { 20, ViewEnum::SetValueViewEnum },     // OperatingModeSetDurationClock
    { 21, ViewEnum::SetValueViewEnum },     // OperatingModeSetDurationDate
    { 22, ViewEnum::SetValueViewEnum },     // OperatingModeSetDurationTemp
    { 23, ViewEnum::SetValueViewEnum },     // OperatingModeSetDurationFade
    { 24, ViewEnum::SetValueViewEnum },     // OperatingModeDstBeginMonth
    { 25, ViewEnum::SetValueViewEnum },     // OperatingModeDstBeginDowOrdinal
    { 26, ViewEnum::SetValueViewEnum },     // OperatingModeDstEndMonth
    { 27, ViewEnum::SetValueViewEnum },     // OperatingModeDstEndDowOrdinal
    { 28, ViewEnum::SetValueViewEnum },     // OperatingModeDstSwitchDayOfWeek
    { 29, ViewEnum::SetValueViewEnum },     // OperatingModeDstSwitchHour
    { 30, ViewEnum::SetValueViewEnum },     // OperatingModeSetMinimumIntensity
    { 31, ViewEnum::SetValueViewEnum },     // OperatingModeSetFlickerReduction
    { 32, ViewEnum::SetValueViewEnum },     // OperatingModeSetCurrentDrive
    { 33, ViewEnum::SetValueViewEnum },     // OperatingModeSetTempCalibration
    { 34, ViewEnum::SetValueViewEnum },     // OperatingModeSetDMX512Address
    { 40, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot1Time
    { 41, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot2Time
    { 42, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot3Time
    { 43, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot4Time
    { 44, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot5Time
    { 45, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot6Time
    { 46, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot7Time
    { 47, ViewEnum::SetTimeDateViewEnum },  // OperatingModeSlot8Time
    { 50, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot1Colors
    { 51, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot2Colors
    { 52, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot3Colors
    { 53, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot4Colors
    { 54, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot5Colors
    { 55, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot6Colors
    { 56, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot7Colors
    { 57, ViewEnum::SetColorsViewEnum },    // OperatingModeSlot8Colors
    { 58, ViewEnum::SetColorsViewEnum },    // OperatingModeDateColors
    { 59, ViewEnum::SetColorsViewEnum },    // OperatingModeTempColors
    { 60, ViewEnum::SetColorsViewEnum }     // OperatingModeTimerCounterColors
};

// The loop delay in milliseconds.
//
// static const uint8_t cLoopDelay = 20;

// The maximum idle time before the application switches back to the default view
//
static const uint32_t cMaximumIdleCount = 100000;

// The current mode and sub-mode (if used) of the application
//
OperatingMode _applicationMode;

// The application's current settings
//
Settings _settings;

// The current displayed view of the application
//
// View *_currentView = cModeViews[static_cast<uint8_t>(OperatingMode::OperatingModeFixedDisplay)];
View *_currentView = cModeViews[static_cast<uint8_t>(ViewEnum::TimeDateTempViewEnum)];

// Idle cycle counter
//
uint32_t _idleCounter = 0;


// Initializes the application
//
void initialize()
{
  Hardware::autoAdjustIntensities(true);

  _settings.loadFromFlash();

  // Update alarms
  AlarmHandler::setSettings(_settings);

  // Update hardware things
  Hardware::autoAdjustIntensities(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));
  Hardware::setFlickerReduction(_settings.getRawSetting(Settings::Setting::FlickerReduction));
  Hardware::currentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setMinimumIntensity(_settings.getRawSetting(Settings::Setting::MinimumIntensity));
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  setMode(OperatingMode::OperatingModeFixedDisplay);
}


uint8_t getModeDisplayNumber(OperatingMode mode)
{
  return cViewDescriptor[static_cast<uint8_t>(mode)].displayNumber;
}


uint8_t getModeDisplayNumber(uint8_t mode)
{
  if (mode <= static_cast<uint8_t>(OperatingMode::OperatingModeTimerCounterColors))
  {
    return cViewDescriptor[mode].displayNumber;
  }
  return 0;
}


// Allows views to get the application operating mode
//
OperatingMode getMode()
{
  return _applicationMode;
}


// Sets the application operating mode
//
void setMode(OperatingMode mode)
{
  if (_applicationMode != mode)
  {
    _applicationMode = mode;
    _idleCounter = 0;
    _currentView = cModeViews[static_cast<uint8_t>(cViewDescriptor[static_cast<uint8_t>(mode)].view)];
    _currentView->enter();
  }
}


// Allows other views to get the active settings
//
Settings getSettings()
{
  return _settings;
}

// Apply new settings
//
void setSettings(Settings settings)
{
  _settings = settings;

  // Update alarms
  AlarmHandler::setSettings(settings);

  // Update hardware things
  Hardware::autoAdjustIntensities(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));
  Hardware::setFlickerReduction(_settings.getRawSetting(Settings::Setting::FlickerReduction));
  Hardware::currentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setMinimumIntensity(_settings.getRawSetting(Settings::Setting::MinimumIntensity));
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  _settings.saveToFlash();
}


// Here lies the main application loop
//
void loop()
{
  while(true)
  {
    // Refresh hardware data (date, time, temperature, etc.)
    Hardware::refresh();

    // Check the buttons
    Keys::scanKeys();
    if (Keys::hasKeyPress())
    {
      // Get the key from the buffer and process it
      auto key = Keys::getKeyPress();

      // If an alarm is active, send keypresses there
      if (AlarmHandler::isAlarmActive())
      {
        AlarmHandler::keyHandler(key);
      }
      else
      {
        _currentView->keyHandler(key);
      }
      // Reset the counter since there has been button/key activity
      _idleCounter = 0;
    }
    // Execute the loop block of the current view
    _currentView->loop();
    // Process any necessary alarms
    AlarmHandler::loop();

    // If the idle counter has maxed out, kick us back to the toggling display
    if (_idleCounter++ > cMaximumIdleCount)
    {
      _idleCounter = 0;
      if (_applicationMode == OperatingMode::OperatingModeMainMenu)
      {
        setMode(OperatingMode::OperatingModeToggleDisplay);
      }
    }
    // Hardware::delay(cLoopDelay);
  }
}


}

}
