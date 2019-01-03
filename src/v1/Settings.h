//
// kbx81's binary clock settings class
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

#include "DateTime.h"
#include "Hardware.h"
#include "RgbLed.h"


namespace kbxBinaryClock {

class Settings {

public:

  /// @brief Types of settings we keep
  ///
  enum Setting : uint8_t
  {
    SystemOptions = 0,
    BeepStates = 1,
    BlinkStates = 2,
    ColorStates = 3,
    TimeDisplayDuration = 4,
    DateDisplayDuration = 5,
    TemperatureDisplayDuration = 6,
    FadeDuration = 7,
    DstBeginMonth = 8,
    DstBeginDowOrdinal = 9,
    DstEndMonth = 10,
    DstEndDowOrdinal = 11,
    DstSwitchDayOfWeek = 12,
    DstSwitchHour = 13,
    MinimumIntensity = 14,
    FlickerReduction = 15,
    CurrentDrive = 16,
    TemperatureCalibration = 17,
    BeeperVolume = 18,
    TimerResetValue = 19,
    DisplayRefreshInterval = 20,
    DmxAddress = 21
  };

  /// @brief Settings we keep
  ///
  enum SystemOptionsBits : uint8_t
  {
    DisplayBCD = 0,
    Display12Hour = 1,
    StatusLedAsAmPm = 2,
    HourlyChime = 3,
    DstEnable = 4,
    DisplayFahrenheit = 5,
    FadeAcrossSlots = 6,
    AutoAdjustIntensity = 7,
    DmxExtended = 8,
    StartupToToggle = 9,
    MSBsOff = 10
  };

  /// @brief Enums for time slots
  ///
  enum Slot : uint8_t
  {
    Slot1 = 0,
    Slot2 = 1,
    Slot3 = 2,
    Slot4 = 3,
    Slot5 = 4,
    Slot6 = 5,
    Slot7 = 6,
    Slot8 = 7,
    SlotDate = 8,
    SlotTemperature = 9,
    SlotTimer = 10,
    SlotMenu = 11,
    SlotSet = 12,
    SlotDmx = 13,
    SlotCalculated = 14
  };


  /// @brief Enums for colors
  ///
  enum Color : uint8_t
  {
    Color0 = 0,
    Color1 = 1
  };


public:

  /// @brief Where settings will be written in FLASH
  ///
  static const uint32_t cSettingsFlashAddress;

  /// @brief Bits used in each settings class
  ///
  static const uint16_t cSettingData[];

  /// @brief Default constructor
  ///
  Settings();


  /// @brief Initialize settings
  ///
  void initialize();

  /// @brief Loads settings from FLASH
  /// @return true if success, false if failure & defaults were set
  ///
  bool loadFromFlash();

  /// @brief Saves settings to FLASH
  /// @return result of writeFlash()
  ///
  uint32_t saveToFlash();


  /// @brief Returns a setting
  /// @return true if setting is enabled, false otherwise
  ///
  bool getSetting(const uint8_t setting, const uint8_t item);

  /// @brief Sets a setting
  ///
  void setSetting(const uint8_t setting, const uint8_t item, const bool state);

  /// @brief Returns all settings
  /// @return value of requested setting
  ///
  uint16_t getRawSetting(const uint8_t setting);

  /// @brief Sets all settings
  ///
  void setRawSetting(const uint8_t setting, const uint16_t value);


  /// @brief Get a time value stored in the settings
  ///
  DateTime getTime(const Slot setting);
  DateTime getTime(const uint8_t setting);

  /// @brief Set a time value in the settings
  ///
  void setTime(const Slot setting, const DateTime time);
  void setTime(const uint8_t setting, const DateTime time);


  /// @brief Gets a time slot's color value
  ///
  RgbLed getColor(const Color color, const Slot slot, const bool setFadeDuration = false);
  RgbLed getColor(const uint8_t color, const uint8_t slot, const bool setFadeDuration = false);

  /// @brief Set a time slot's color values in the settings
  ///
  void setColors(const Slot slot, const RgbLed color0, const RgbLed color1);
  void setColors(const uint8_t slot, const RgbLed color0, const RgbLed color1);

  /// @brief Update the colors used when fading across time slots
  ///
  void refreshCalculatedColors();


private:

  /// @brief array of values for various settings data
  ///
  uint16_t _setting[Setting::DmxAddress + 1];

  /// @brief array of stored times for the alarms and/or color changes
  ///
  DateTime _time[Slot::Slot8 + 1];

  /// @brief two arrays of colors...one for '0' and one for '1'
  ///
  RgbLed _color[2][Slot::SlotCalculated + 1];

  /// @brief determines validity of Settings data after loading from FLASH
  ///
  uint32_t _crc;
};

}
