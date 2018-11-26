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

  // @brief Types of settings we keep
  //
  enum Setting : uint8_t
  {
    SystemOptions = 0,
    BeepStates = 1,
    BlinkStates = 2,
    ColorStates = 3,
    TimeDisplayDuration = 4,
    DateDisplayDuration = 5,
    TemperatureDisplayDuration = 6,
    FadeRate = 7,
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
    DmxAddress = 20
  };

  // @brief Settings we keep
  //
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

  // @brief Enums for time slots
  //
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


public:

  // @brief Where settings will be written in FLASH
  //
  static const uint32_t cSettingsFlashAddress;

  // @brief Bits used in each settings class
  //
  static const uint32_t cSettingsValidationKey;

  // @brief Bits used in each settings class
  //
  static const uint16_t cSettingData[];

  // @brief Default constructor
  //
  Settings();


  // Initialize settings
  //
  void initialize();

  // Loads settings from FLASH
  //
  void loadFromFlash();

  // Saves settings to FLASH
  //
  uint32_t saveToFlash();


  // Returns a setting
  //
  bool getSetting(const uint8_t setting, const uint8_t item);

  // Sets a setting
  //
  void setSetting(const uint8_t setting, const uint8_t item, const bool state);

  // Returns all settings
  //
  uint16_t getRawSetting(const uint8_t setting);

  // Sets all settings
  //
  void setRawSetting(const uint8_t setting, const uint16_t value);


  // Get a time value stored in the settings
  //
  DateTime getTime(const Slot setting);
  DateTime getTime(const uint8_t setting);

  // Set a time value in the settings
  //
  void setTime(const Slot setting, const DateTime time);
  void setTime(const uint8_t setting, const DateTime time);


  // Gets a time slot's color0 value
  //
  RgbLed getColor0(const Slot slot);
  RgbLed getColor0(const uint8_t slot);

  // Gets a time slot's color1 value
  //
  RgbLed getColor1(const Slot slot);
  RgbLed getColor1(const uint8_t slot);

  // Set a time slot's color values in the settings
  //
  void setColors(const Slot slot, const RgbLed color0, const RgbLed color1);
  void setColors(const uint8_t slot, const RgbLed color0, const RgbLed color1);

  // Update the colors used when fading across time slots
  //
  void refreshCalculatedColors();


private:

  // array of settings data
  //
  uint16_t _setting[Setting::DmxAddress + 1];

  // array of stored times
  //
  DateTime _time[Slot::Slot8 + 1];

  // arrays of colors
  //
  RgbLed _color0[Slot::SlotCalculated + 1];
  RgbLed _color1[Slot::SlotCalculated + 1];

  // indicates validity of Settings data
  //  used to determine if load from FLASH worked
  uint32_t _validityKey;
};

}
