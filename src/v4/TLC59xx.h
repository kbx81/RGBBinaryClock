//
// kbx81's binary clock TLC59xx LED device class
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

#include "Hardware.h"


namespace kbxBinaryClock {

class TLC59xx {

public:
  // @brief Type of device(s) connected
  //
  enum DeviceType : uint8_t
  {
    DeviceTLC5947,
    DeviceTLC5951
  };

  // @brief Reversing mode for channel/LED data
  //
  enum ReversingMode : uint8_t
  {
    NoReversing,
    ReversePerDevice,
    ReverseAcrossBuffer,
    ReversePerDeviceAcrossBuffer
  };

  // @brief Dot Correction Adjustment Ranges
  //
  enum DcAdjRange : uint8_t
  {
    DcRange0to66 = 0,
    DcRange33to100 = 1
  };

  // @brief Greyscale Resolutions
  //
  enum GreyscaleResolution : uint8_t
  {
    Gs12BitMode = 0,
    Gs12BitMode1 = 1,
    Gs10BitMode = 2,
    Gs8BitMode = 3
  };

  // @brief TLC5951 Function Control Register bits
  //
  enum TLC5951FcRegisterBits : uint8_t
  {
    TLC5951DcRangeRed = 0,
    TLC5951DcRangeGreen = 1,
    TLC5951DcRangeBlue = 2,
    TLC5951DispRepMode = 3,
    TLC5951TimingResetMode = 4,
    TLC5951GsResolutionBitmaskLow = 5,
    TLC5951GsResolutionBitmaskHigh = 6,
    TLC5951UserBit = 7
  };

typedef DeviceType tDeviceType;
typedef ReversingMode tReversingMode;
typedef DcAdjRange tDcAdjRange;
typedef GreyscaleResolution tGreyscaleResolution;

public:

  // number of PWM channels each device has
  //
  static const uint8_t cTLC5947PwmChannelsPerDevice;
  static const uint8_t cTLC5951PwmChannelsPerDevice;

  // size (in bytes) of device shift registers
  //
  static const uint8_t cTLC5947ShiftRegSize;
  static const uint8_t cTLC5951ShiftRegSize;
  static const uint8_t cTLC5951DcBcFcUdShiftRegSize;

  // number of bytes needed per PWM channel
  //
  static const uint8_t cPwmBytesPerChannel;

  // maximum value for PWM channels
  //
  static const uint16_t cPwmMaximumValue;


  // @brief Constructor
  //
  TLC59xx(const DeviceType type = DeviceType::DeviceTLC5947, const uint16_t numberOfDevices = 1, const bool shareCommonRegister = true, const ReversingMode reversingMode = ReversingMode::NoReversing);

  // @brief Sets a single PWM channel to the given value
  //
  void setPwmChannel(const uint16_t channel, const uint16_t value);

  // @brief Sets a triad (red, green, blue) of PWM channels to the given values
  //
  void setPwmTriad(const uint16_t ledNumber, const uint16_t red, const uint16_t green, const uint16_t blue);

  // @brief Gets/Sets the dot correction value for a single output
  //  TLC5951 ONLY
  uint8_t getDcChannel(const uint16_t channel);
  void setDcChannel(const uint16_t channel, const uint8_t value);

  // @brief Sets the dot correction values for a triad (red, green, blue) of outputs
  //  TLC5951 ONLY
  void setDcTriad(const uint16_t ledNumber, const uint8_t red, const uint8_t green, const uint8_t blue);

  // @brief Gets/Sets the global brightness control value for a device
  //  TLC5951 ONLY
  uint8_t getBcRed(const uint8_t deviceNumber);
  uint8_t getBcGreen(const uint8_t deviceNumber);
  uint8_t getBcBlue(const uint8_t deviceNumber);
  void setBcRed(const uint8_t deviceNumber, const uint8_t value);
  void setBcGreen(const uint8_t deviceNumber, const uint8_t value);
  void setBcBlue(const uint8_t deviceNumber, const uint8_t value);

  // @brief Sets the global brightness control values for a triad (red, green, blue) of outputs
  //  TLC5951 ONLY
  void setBcTriad(const uint16_t deviceNumber, const uint8_t red, const uint8_t green, const uint8_t blue);

  // @brief Gets/Sets the dot correction range (bit) to the specified range for the given device
  //  TLC5951 ONLY
  tDcAdjRange getDcRangeRed(const uint8_t deviceNumber);
  tDcAdjRange getDcRangeGreen(const uint8_t deviceNumber);
  tDcAdjRange getDcRangeBlue(const uint8_t deviceNumber);
  void setDcRangeRed(const uint8_t deviceNumber, const DcAdjRange range);
  void setDcRangeGreen(const uint8_t deviceNumber, const DcAdjRange range);
  void setDcRangeBlue(const uint8_t deviceNumber, const DcAdjRange range);

  // @brief Gets/Sets the auto display repeat mode for the given device (see RM)
  //  TLC5951 ONLY
  bool autoRepeatIsEnabled(const uint8_t deviceNumber);
  void setAutoRepeatMode(const uint8_t deviceNumber, const bool autoRepeatEnabled);

  // @brief Gets/Sets the display timing reset mode for the given device (see RM)
  //  TLC5951 ONLY
  bool timingResetModeIsEnabled(const uint8_t deviceNumber);
  void setTimingResetMode(const uint8_t deviceNumber, const bool autoResetEnabled);

  // @brief Gets/Sets the PWM resolution for the given device to the value specified
  //  TLC5951 ONLY
  tGreyscaleResolution getPwmResolution(const uint8_t deviceNumber);
  void setPwmResolution(const uint8_t deviceNumber, const GreyscaleResolution resolution);

  // @brief Gets/Sets upper 16 user data (UD) bits for the device specified
  //  TLC5951 ONLY
  uint16_t getUserData(const uint8_t deviceNumber);
  void setUserData(const uint8_t deviceNumber, const uint16_t data);

  // @brief Returns varios SID data (see RM)
  //  TLC5951 ONLY -- DOES NOT CONSIDER REVERSING MODE
  bool isOutputOpen(const uint16_t channel);
  bool isOutputShorted(const uint16_t channel);
  bool isThermalErrorActive(const uint8_t deviceNumber);

  // @brief Gets/Sets reversing mode
  // *** Does NOT modify/reverse existing data! Only affects new data writes ***
  tReversingMode getReversingMode();
  void setReversingMode(ReversingMode newReversingMode);

  // @brief Returns true if one or more values have changed and need to be
  //  written out to the hardware. Clears the flag if clearFlag is true
  bool gsHwRefreshRequired(const bool clearFlag);
  bool dcBcFcUdHwRefreshRequired(const bool clearFlag);

  // @brief Set the corresponding refreshRequired flag
  //
  void setGsHwRefreshRequired();
  void setDcBcFcUdHwRefreshRequired();

  // @brief Returns a pointer to the buffer for writing out the SPI peripheral
  //
  uint16_t* getPwmBufferPtrForWrite();
  //  TLC5951 ONLY
  uint8_t* getDcBcFcUdBufferPtrForWrite();

private:

  static const uint8_t cTLC5951UdBaseIndex;
  static const uint8_t cTLC5951FcBaseIndex;
  static const uint8_t cTLC5951BcBaseIndex;
  static const uint8_t cTLC5951DcBaseIndex;
  static const uint8_t cTLC5951LodBaseIndex;
  static const uint8_t cTLC5951LsdBaseIndex;

  ReversingMode _reversingMode;

  uint16_t* _pPwmBuffer;
  uint8_t* _pDcBcFcUdBuffer;

  uint16_t _numberOfDevices;

  uint8_t _pwmChannelsPerDevice;
  uint8_t _DcBcFcUdShiftRegSize;

  uint8_t _dataOffset;

  bool _gsHwRefreshRequired;
  bool _dcBcFcUdHwRefreshRequired;

  uint16_t _getModifiedChannel(const uint16_t channel);
};


}
