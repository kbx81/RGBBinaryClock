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
#include "DateTime.h"
#include "Dmx-512-Controller.h"
#include "Dmx-512-Rx.h"
#include "Dmx-512-View.h"
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
    new Dmx512View(),
    new SetTimeDateView(),
    new SetBitsView(),
    new SetValueView(),
    new SetColorsView()
};

// Structure defining menu views
//
struct viewDescriptor {
  uint8_t  menuItemDisplayNumber;
  ViewEnum view;
};


// An array of mode display numbers and their corresponding views
//
static viewDescriptor const cViewDescriptor[] = {
    {  0, ViewEnum::MainMenuViewEnum },     // OperatingModeMainMenu
    {  1, ViewEnum::TimeDateTempViewEnum }, // OperatingModeFixedDisplay
    {  2, ViewEnum::TimeDateTempViewEnum }, // OperatingModeToggleDisplay
    {  3, ViewEnum::TimerCounterViewEnum }, // OperatingModeTimerCounter
    {  4, ViewEnum::Dmx512ViewEnum },       // OperatingModeDmx512Display
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
    { 34, ViewEnum::SetValueViewEnum },     // OperatingModeSetBeeperVolume
    { 39, ViewEnum::SetValueViewEnum },     // OperatingModeSetDMX512Address
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
    { 60, ViewEnum::SetColorsViewEnum },    // OperatingModeTimerCounterColors
    { 61, ViewEnum::SetColorsViewEnum },    // OperatingModeMenuColors
    { 62, ViewEnum::SetColorsViewEnum },    // OperatingModeSetColors
};

// The maximum idle time before the application switches back to the default view
//
static const uint32_t cMaximumIdleCount = 80000;

// The maximum idle time before the application switches back to the default view
//
static const uint8_t cMinimumIntensityMultiplier = 10;

// important daylight savings dates & times
//
DateTime _currentTime, _dstStart, _dstEnd;

// daylight savings state
//
DstState _dstState;

// The current mode of the application
//
OperatingMode _applicationMode;

// The application's current view mode
//
ViewMode _viewMode;

// The application's current external control mode
//
ExternalControl _externalControlMode = ExternalControl::NoActiveExtControlEnum;

// The application's current settings
//
Settings _settings;

// The current displayed view of the application
//
// View *_currentView = cModeViews[static_cast<uint8_t>(OperatingMode::OperatingModeFixedDisplay)];
View *_currentView = cModeViews[static_cast<uint8_t>(ViewEnum::TimeDateTempViewEnum)];

// DMX-512 signal status the last time we checked
//
bool _previousDmxState = false;

// Idle cycle counter
//
uint32_t _idleCounter = 0;


// Initializes the application
//
void initialize()
{
  Hardware::autoAdjustIntensities(true);

  _settings.loadFromFlash();

  _currentTime = Hardware::getDateTime();
  // Update DST start and end dates and times based on RTC
  isDst(_currentTime);

  // Update alarms
  AlarmHandler::initialize();
  AlarmHandler::setSettings(_settings);

  // Update hardware things
  Hardware::autoAdjustIntensities(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));
  Hardware::setFlickerReduction(_settings.getRawSetting(Settings::Setting::FlickerReduction));
  Hardware::setVolume(_settings.getRawSetting(Settings::Setting::BeeperVolume));
  Hardware::currentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setMinimumIntensity(_settings.getRawSetting(Settings::Setting::MinimumIntensity) * cMinimumIntensityMultiplier);
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  Dmx512Controller::initialize();

  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StartupToToggle) == true)
  {
    setOperatingMode(OperatingMode::OperatingModeToggleDisplay);
  }
  else
  {
    setOperatingMode(OperatingMode::OperatingModeFixedDisplay);
  }
}


uint8_t getModeDisplayNumber(OperatingMode mode)
{
  return cViewDescriptor[static_cast<uint8_t>(mode)].menuItemDisplayNumber;
}


uint8_t getModeDisplayNumber(uint8_t mode)
{
  if (mode <= static_cast<uint8_t>(OperatingMode::OperatingModeSetColors))
  {
    return cViewDescriptor[mode].menuItemDisplayNumber;
  }
  return 0;
}


// Allows views to get the application operating mode
//
OperatingMode getOperatingMode()
{
  return _applicationMode;
}


// Sets the application operating mode
//
void setOperatingMode(OperatingMode mode)
{
  if (_applicationMode != mode)
  {
    _applicationMode = mode;
    _viewMode = ViewMode::ViewMode0;
    _currentView = cModeViews[static_cast<uint8_t>(cViewDescriptor[static_cast<uint8_t>(mode)].view)];
    _currentView->enter();

    if (mode == OperatingMode::OperatingModeMainMenu)
    {
      _idleCounter = 0;
    }
  }
}


// Allows views to get their desired mode
//
ViewMode getViewMode()
{
  return _viewMode;
}


// Sets the view's desired mode
//
void setViewMode(ViewMode mode)
{
  if (_viewMode != mode)
  {
    _viewMode = mode;
  }
}


// Get external control status
//
ExternalControl getExternalControlState()
{
  return _externalControlMode;
}


// Allows other views to get the active settings
//
Settings getSettings()
{
  return _settings;
}


// Allows other views to get a pointer to the application's active settings
//
Settings* getSettingsPtr()
{
  return &_settings;
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
  Hardware::setVolume(_settings.getRawSetting(Settings::Setting::BeeperVolume));
  Hardware::currentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setMinimumIntensity(_settings.getRawSetting(Settings::Setting::MinimumIntensity) * cMinimumIntensityMultiplier);
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  Dmx512Controller::initialize();

  _settings.saveToFlash();
}


// Handles DST date/time computation and setup
//
bool isDst(const DateTime &currentTime)
{
  uint8_t firstDay;

  if (_dstStart.year(false) != currentTime.year(false))
  {
    _dstState = DstState::Reset;
    firstDay = ((_settings.getRawSetting(Settings::Setting::DstBeginDowOrdinal) - 1) * 7) + 1;

    do
    {
      _dstStart.setDate(currentTime.year(false), _settings.getRawSetting(Settings::Setting::DstBeginMonth), firstDay++);
    }
    while((_dstStart.dayOfWeek() != _settings.getRawSetting(Settings::Setting::DstSwitchDayOfWeek)) && (firstDay <= 31));

    _dstStart.setTime(_settings.getRawSetting(Settings::Setting::DstSwitchHour), 0, 0);

    if (currentTime >= _dstStart)
    {
      _dstState = DstState::Spring;
    }
  }

  if (_dstEnd.year(false) != currentTime.year(false))
  {
    firstDay = ((_settings.getRawSetting(Settings::Setting::DstEndDowOrdinal) - 1) * 7) + 1;

    do
    {
      _dstEnd.setDate(currentTime.year(false), _settings.getRawSetting(Settings::Setting::DstEndMonth), firstDay++);
    }
    while((_dstEnd.dayOfWeek() != _settings.getRawSetting(Settings::Setting::DstSwitchDayOfWeek)) && (firstDay <= 31));

    _dstEnd.setTime(_settings.getRawSetting(Settings::Setting::DstSwitchHour), 0, 0);

    if (currentTime >= _dstEnd)
    {
      _dstState = DstState::Fall;
    }
  }

  // once everything is calculated above, it becomes this simple...
  return (currentTime >= _dstStart && currentTime < _dstEnd);
}


// Handles updating of the clock hardware for DST
//
void _refreshDst()
{
  _currentTime = Hardware::getDateTime();

  if ((_currentTime >= _dstEnd) && (_dstState == DstState::Spring))
  {
    _dstState = DstState::Fall;
    Hardware::setDstState(isDst(_currentTime), true);
  }

  if ((_currentTime >= _dstStart) && (_dstState == DstState::Reset))
  {
    _dstState = DstState::Spring;
    Hardware::setDstState(isDst(_currentTime), true);
  }
}


// Here lies the main application loop
//
void loop()
{
  while(true)
  {
    // Refresh hardware data (date, time, temperature, etc.)
    Hardware::refresh();

    // Check up on DST and adjust the time if enabled based on settings
    if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DstEnable) == true)
    {
      _refreshDst();
    }

    // Check the buttons
    Keys::scanKeys();
    if (Keys::hasKeyPress())
    {
      Hardware::tick();
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

    if (Dmx512Rx::signalIsActive() != _previousDmxState)
    {
      _previousDmxState = Dmx512Rx::signalIsActive();
      if (_previousDmxState == true)
      {
        _externalControlMode = ExternalControl::Dmx512ExtControlEnum;
      }
      else
      {
        _externalControlMode = ExternalControl::NoActiveExtControlEnum;
        Hardware::displayBlank(false);
      }
      setOperatingMode(OperatingMode::OperatingModeDmx512Display);
    }

    // If the idle counter has maxed out, kick us back to the toggling display
    if (_idleCounter++ > cMaximumIdleCount)
    {
      _idleCounter--;
      if (_applicationMode == OperatingMode::OperatingModeMainMenu)
      {
        if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::StartupToToggle) == true)
        {
          setOperatingMode(OperatingMode::OperatingModeToggleDisplay);
        }
        else
        {
          setOperatingMode(OperatingMode::OperatingModeFixedDisplay);
        }
      }
      // Allow the DMX-512 controller to do its thing
      Dmx512Controller::controller();
    }
    else if (_applicationMode == OperatingMode::OperatingModeDmx512Display)
    {
      // Allow the DMX-512 controller to do its thing
      Dmx512Controller::controller();
    }
  }
}


}

}
