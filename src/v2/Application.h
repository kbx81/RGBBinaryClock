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
#pragma once


#include <cstdint>

#include "Settings.h"
#include "View.h"


namespace kbxBinaryClock {

namespace Application {

  /// @brief Application operating modes
  ///
  enum OperatingMode : uint8_t
  {
    OperatingModeMainMenu,            ///< Main Menu mode
    OperatingModeFixedDisplay,        ///< Fixed display mode
    OperatingModeToggleDisplay,       ///< Toggling display mode
    OperatingModeTimerCounter,        ///< Timer/counter mode
    OperatingModeDmx512Display,       ///< DMX-512 display mode
    OperatingModeSetClock,            ///< Set timeclock mode
    OperatingModeSetDate,             ///< Set date mode
    OperatingModeSetSystemOptions,    ///< Set Binary/BCD and 24 hour bits mode
    OperatingModeSlotBeepConfig,      ///< Enable/Disable beeping (alarm) per-slot
    OperatingModeSlotBlinkConfig,     ///< Enable/Disable display blinking per-slot
    OperatingModeSlotColorConfig,     ///< Enable/Disable color change per-slot
    OperatingModeSetDurationClock,    ///< Set duration of clock display mode
    OperatingModeSetDurationDate,     ///< Set duration of date display mode
    OperatingModeSetDurationTemp,     ///< Set duration of temperature display mode
    OperatingModeSetDurationFade,     ///< Set duration of fading for digits
    OperatingModeDstBeginMonth,       ///< Set first month of DST
    OperatingModeDstBeginDowOrdinal,  ///< Set which given day DST begins
    OperatingModeDstEndMonth,         ///< Set last month of DST
    OperatingModeDstEndDowOrdinal,    ///< Set which given day DST ends
    OperatingModeDstSwitchDayOfWeek,  ///< Set which day of week DST begins/ends
    OperatingModeDstSwitchHour,       ///< Set hour at which DST begins/ends
    OperatingModeSetMinimumIntensity, ///< Set minimum display intensity
    OperatingModeSetFlickerReduction, ///< Set flicker reduction value for display
    OperatingModeSetCurrentDrive,     ///< Set drive current for LEDs
    OperatingModeSetTempCalibration,  ///< Set temperature calibration
    OperatingModeSetBeeperVolume,     ///< Set Beeper Volume
    OperatingModeSetDMX512Address,    ///< Set DMX-512 address mode
    OperatingModeSlot1Time,           ///< Set slot 1 time mode
    OperatingModeSlot2Time,           ///< Set slot 2 time mode
    OperatingModeSlot3Time,           ///< Set slot 3 time mode
    OperatingModeSlot4Time,           ///< Set slot 4 time mode
    OperatingModeSlot5Time,           ///< Set slot 5 time mode
    OperatingModeSlot6Time,           ///< Set slot 6 time mode
    OperatingModeSlot7Time,           ///< Set slot 7 time mode
    OperatingModeSlot8Time,           ///< Set slot 8 time mode
    OperatingModeSlot1Colors,         ///< Set slot 1 colors mode
    OperatingModeSlot2Colors,         ///< Set slot 2 colors mode
    OperatingModeSlot3Colors,         ///< Set slot 3 colors mode
    OperatingModeSlot4Colors,         ///< Set slot 4 colors mode
    OperatingModeSlot5Colors,         ///< Set slot 5 colors mode
    OperatingModeSlot6Colors,         ///< Set slot 6 colors mode
    OperatingModeSlot7Colors,         ///< Set slot 7 colors mode
    OperatingModeSlot8Colors,         ///< Set slot 8 colors mode
    OperatingModeDateColors,          ///< Set date colors mode
    OperatingModeTempColors,          ///< Set temperature colors mode
    OperatingModeTimerCounterColors,  ///< Set timer/counter colors mode
    OperatingModeMenuColors,          ///< Set menu colors mode
    OperatingModeSetColors            ///< Set set colors mode
  };

  /// @brief Enums for views
  ///
  enum ViewEnum : uint8_t
  {
      MainMenuViewEnum,
      TimeDateTempViewEnum,
      TimerCounterViewEnum,
      Dmx512ViewEnum,
      SetTimeDateViewEnum,
      SetBitsViewEnum,
      SetValueViewEnum,
      SetColorsViewEnum
  };

  /// @brief Enums for views
  ///
  enum ExternalControl : uint8_t
  {
      NoActiveExtControlEnum,
      SerialExtControlEnum,
      Dmx512ExtControlEnum
  };


  // Initialize the application
  //
  void initialize();

  // Get a mode's display number
  //
  uint8_t getModeDisplayNumber(OperatingMode mode);
  uint8_t getModeDisplayNumber(uint8_t mode);

  // Get application mode
  //
  OperatingMode getOperatingMode();

  // Set new application mode
  //
  void setOperatingMode(OperatingMode mode);

  // Get view mode
  //
  ViewMode getViewMode();

  // Set new view mode
  //
  void setViewMode(ViewMode mode);

  // Get external control status
  //
  ExternalControl getExternalControlState();

  // Get application settings
  //
  Settings getSettings();
  Settings* getSettingsPtr();

  // Set new application settings
  //
  void setSettings(Settings settings);

  // The main loop of the application.
  //
  __attribute__((noreturn))
  void loop();
}

}
