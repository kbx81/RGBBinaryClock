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
#include <stdlib.h>

#include "Application.h"
#include "AlarmHandler.h"
#include "DateTime.h"
#include "Display.h"
#include "DisplayManager.h"
#include "Dmx-512-Controller.h"
#include "Dmx-512-Rx.h"
#include "Dmx-512-View.h"
#include "Keys.h"
#include "Settings.h"
#include "MainMenuView.h"
#include "TestDisplayView.h"
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
    new SetColorsView(),
    new TestDisplayView()
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
    { 35, ViewEnum::SetValueViewEnum },     // OperatingModeSetTimerResetValue
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
    { 98, ViewEnum::TestDisplayEnum }       // OperatingModeTestDisplay
};

// The maximum idle time before the application switches back to the default view
//   Counter is incremented by tick()
static const uint16_t cMaximumIdleCount = 60000;

// A value by which we multiply the preference stored in the Settings
//
static const uint8_t cMinimumIntensityMultiplier = 10;

// Idle cycle counter
//
volatile static uint16_t _idleCounter = cMaximumIdleCount;

// minimum intensity
//   100 = 1%
static uint16_t _minimumIntensity = 100;

// multiplier used to adjust display intensity
//
static uint16_t _intensityPercentage = _minimumIntensity;

// important daylight savings dates & times
//
static DateTime _currentTime, _dstStart, _dstEnd;

// daylight savings state
//
static DstState _dstState;

// The current mode of the application
//
static OperatingMode _applicationMode;

// The application's current view mode
//
static ViewMode _viewMode;

// The application's current external control mode
//
static ExternalControl _externalControlMode = ExternalControl::NoActiveExtControlEnum;

// The application's current settings
//
static Settings _settings;

// The current displayed view of the application
//
// View *_currentView = cModeViews[static_cast<uint8_t>(OperatingMode::OperatingModeFixedDisplay)];
static View *_currentView = cModeViews[static_cast<uint8_t>(ViewEnum::TimeDateTempViewEnum)];

// determines if display intensity is adjusted in real-time
//
static bool _autoAdjustIntensities = false;

// DMX-512 signal status the last time we checked
//
static bool _previousDmxState = false;

// True if settings need to be written to FLASH
//
static bool _settingsModified = false;


// Initializes the application
//
void initialize()
{
  _settings.loadFromFlash();

  _currentTime = Hardware::getDateTime();
  // Update DST start and end dates and times based on RTC
  isDst(_currentTime);

  // Update alarms
  AlarmHandler::initialize();
  AlarmHandler::setSettings(_settings);

  // Update hardware things
  Hardware::setFlickerReduction(_settings.getRawSetting(Settings::Setting::FlickerReduction));
  Hardware::setVolume(_settings.getRawSetting(Settings::Setting::BeeperVolume));
  Hardware::setLedCurrentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  _minimumIntensity = _settings.getRawSetting(Settings::Setting::MinimumIntensity) * cMinimumIntensityMultiplier;
  setIntensityAutoAdjust(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));

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
  if (mode <= static_cast<uint8_t>(OperatingMode::OperatingModeTestDisplay))
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
    const uint16_t writeLedIntensity = 1024;
    // first, write settings to FLASH, if needed
    if ((mode != OperatingMode::OperatingModeMainMenu) &&
        (mode < static_cast<uint8_t>(OperatingMode::OperatingModeSetSystemOptions)) &&
        (_settingsModified == true))
    {
      if (_settings.saveToFlash() == 0)
      {
        _settingsModified = false;
        Hardware::setGreenLed(writeLedIntensity);
      }
      else
      {
        Hardware::setRedLed(writeLedIntensity);
      }
      DisplayManager::doubleBlink();
      Hardware::setGreenLed(0);
      Hardware::setRedLed(0);
    }
    // set the new mode
    _applicationMode = mode;
    _viewMode = ViewMode::ViewMode0;
    _currentView = cModeViews[static_cast<uint8_t>(cViewDescriptor[static_cast<uint8_t>(mode)].view)];
    _currentView->enter();
    // if something kicks back to the main menu, make sure we wait a full cycle
    if (mode == OperatingMode::OperatingModeMainMenu)
    {
      _idleCounter = 0;
    }
    // this enables controller() immediately upon entering this mode
    else if (mode == OperatingMode::OperatingModeDmx512Display)
    {
      _idleCounter = cMaximumIdleCount;
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
  Hardware::setFlickerReduction(_settings.getRawSetting(Settings::Setting::FlickerReduction));
  Hardware::setVolume(_settings.getRawSetting(Settings::Setting::BeeperVolume));
  Hardware::setLedCurrentDrive(_settings.getRawSetting(Settings::Setting::CurrentDrive));
  Hardware::setTemperatureCalibration((int8_t)(-(_settings.getRawSetting(Settings::Setting::TemperatureCalibration))));

  _minimumIntensity = _settings.getRawSetting(Settings::Setting::MinimumIntensity) * cMinimumIntensityMultiplier;
  setIntensityAutoAdjust(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));

  Dmx512Controller::initialize();

  _settingsModified = true;
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


void setIntensityAutoAdjust(const bool enable)
{
  if (enable == true)
  {
    DisplayManager::setMasterIntensity(_intensityPercentage);
  }
  else
  {
    DisplayManager::setMasterIntensity(10000);
  }

  _autoAdjustIntensities = enable;
}


// Updates the "master" intensity for the display based on lightLevel()
//
void _updateIntensityPercentage()
{
  // determine the new value for _intensityPercentage
  uint16_t currentPercentage = (Hardware::lightLevel() * DisplayManager::cIntensityBaseMultiplier) / Display::cLedMaxIntensity;

  // make sure the intensity does not go below the minimum level
  if (currentPercentage < _minimumIntensity)
  {
    currentPercentage = _minimumIntensity;
  }

  if (currentPercentage > _intensityPercentage)
  {
   _intensityPercentage++;
  }
  else if (currentPercentage < _intensityPercentage)
  {
   _intensityPercentage--;
  }
}


void tick()
{
  if (_idleCounter <= cMaximumIdleCount)
  {
    _idleCounter++;
  }

  _updateIntensityPercentage();
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
    // We may auto-adjust the display intensity only if DMX-512 is NOT active
    if ((_autoAdjustIntensities == true) && (_externalControlMode != ExternalControl::Dmx512ExtControlEnum))
    {
      DisplayManager::setMasterIntensity(_intensityPercentage);
    }

    // Check the buttons
    Keys::scanKeys();
    if (Keys::hasKeyPress())
    {
      Hardware::tick();
      // Get the key from the buffer and process it
      auto key = Keys::getKeyPress();
      // Reset the counter since there has been button/key activity
      _idleCounter = 0;
      // If an alarm is active, send keypresses there
      if (AlarmHandler::isAlarmActive())
      {
        AlarmHandler::keyHandler(key);
      }
      else
      {
        _currentView->keyHandler(key);
      }
    }
    // Execute the loop block of the current view
    _currentView->loop();
    // Process any necessary alarms
    AlarmHandler::loop();

    // If the idle counter has maxed out, kick back to the appropriate display mode
    if (_idleCounter >= cMaximumIdleCount)
    {
      // If a DMX-512 signal is NOT present, kick back to one of the time/date/temp modes
      if (Dmx512Rx::signalIsActive() == false)
      {
        // Only change the display mode if it isn't already one of the time/date/temp modes
        if ((_applicationMode != OperatingMode::OperatingModeToggleDisplay) &&
            (_applicationMode != OperatingMode::OperatingModeFixedDisplay) &&
            (_applicationMode != OperatingMode::OperatingModeTimerCounter))
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
      }
      // Allow the DMX-512 controller to do its thing
      Dmx512Controller::controller();
    }

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
        // DisplayManager::setDisplayBlanking(false);
      }
      // Setting this mode activates the view but also kicks us back to the menu if there is no signal
      setOperatingMode(OperatingMode::OperatingModeDmx512Display);
    }
  }
}


}

}
