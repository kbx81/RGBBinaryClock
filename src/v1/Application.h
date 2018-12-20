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
    OperatingModeSetTimerResetValue,  ///< Set timer/counter reset value
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
    OperatingModeSetColors,           ///< Set set colors mode
    OperatingModeTestDisplay          ///< Test display mode
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
      SetColorsViewEnum,
      TestDisplayEnum
  };

  /// @brief Enums for views
  ///
  enum ExternalControl : uint8_t
  {
      NoActiveExtControlEnum,
      SerialExtControlEnum,
      Dmx512ExtControlEnum
  };

  /// @brief Enums for DST computation
  ///
  enum DstState : uint8_t
  {
    Reset,
    Spring,
    Fall
  };


  /// @brief Initialize the application
  ///
  void initialize();

  /// @brief Get a mode's display number
  ///
  uint8_t getModeDisplayNumber(OperatingMode mode);
  uint8_t getModeDisplayNumber(uint8_t mode);

  /// @brief Get application mode
  ///
  OperatingMode getOperatingMode();

  /// @brief Set new application mode
  ///
  void setOperatingMode(OperatingMode mode);

  /// @brief Get view mode
  ///
  ViewMode getViewMode();

  /// @brief Set new view mode
  ///
  void setViewMode(ViewMode mode);

  /// @brief Get external control status
  ///
  ExternalControl getExternalControlState();

  /// @brief Get application settings
  ///
  Settings getSettings();
  Settings* getSettingsPtr();

  /// @brief Set new application settings
  ///
  void setSettings(Settings settings);

  /// @brief Handles DST date/time computation; maintains clock's DST state machine.
  ///  Intended for tracking DST clock adjustments, not for arbitrary use!
  ///  To reset state machine, call with year != year provided when last called.
  ///  Returns true if DST is active based on passed DateTime object
  bool isDst(const DateTime &currentTime);

  /// @brief Returns state of automatic display intensity adjustments
  /// @returns True if automatic intensity adjustment is enabled
  bool getIntensityAutoAdjust();

  /// @brief Enables/disables automagic adjusting of the display intensity based
  ///  on ambient light seen by the phototransistor
  void setIntensityAutoAdjust(const bool enable);

  /// @brief Refreshs master display intensity. Call at fixed intervals.
  ///
  void tick();

  /// @brief The main loop of the application.
  ///
  __attribute__((noreturn))
  void loop();
}

}
