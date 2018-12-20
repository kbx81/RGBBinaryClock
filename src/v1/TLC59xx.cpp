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
#include <stdlib.h>
#include <string.h>

#include "TLC59xx.h"


namespace kbxBinaryClock {


const uint8_t TLC59xx::cTLC5947PwmChannelsPerDevice = 24;
const uint8_t TLC59xx::cTLC5951PwmChannelsPerDevice = 24;

const uint16_t TLC59xx::cPwmMaximumValue = 0xfff;   // 12 bits (4095 decimal)

const uint8_t TLC59xx::cPwmBytesPerChannel = 2;

const uint8_t TLC59xx::cTLC5947ShiftRegSize = cTLC5947PwmChannelsPerDevice * 1.5; // 12 bits per channel
const uint8_t TLC59xx::cTLC5951ShiftRegSize = cTLC5951PwmChannelsPerDevice * 1.5; // 12 bits per channel
const uint8_t TLC59xx::cTLC5951DcBcFcUdShiftRegSize = 27;
const uint8_t TLC59xx::cTLC5951UdBaseIndex = 0;
const uint8_t TLC59xx::cTLC5951FcBaseIndex = 2;
const uint8_t TLC59xx::cTLC5951BcBaseIndex = 3;
const uint8_t TLC59xx::cTLC5951DcBaseIndex = 6;
const uint8_t TLC59xx::cTLC5951LodBaseIndex = 0;
const uint8_t TLC59xx::cTLC5951LsdBaseIndex = 3;


TLC59xx::TLC59xx(const DeviceType type, const uint16_t numberOfDevices, const bool shareCommonRegister, const ReversingMode reversingMode)
{
  uint16_t bufferSize = numberOfDevices;

  _numberOfDevices = numberOfDevices;
  _reversingMode = reversingMode;
  _dataOffset = 0;
  _gsHwRefreshRequired = true;
  _dcBcFcUdHwRefreshRequired = true;

  switch (type)
  {
    case DeviceType::DeviceTLC5951:
      if (shareCommonRegister == true)
      {
        _DcBcFcUdShiftRegSize = cTLC5951ShiftRegSize;
        bufferSize = cTLC5951ShiftRegSize * numberOfDevices;
        // we need 21 bytes for DC and six bytes for BC, FC, and UD, but we also
        //  need to fill out the remaining bits in the common shift register in
        //  case devices are chained. therefore, the first (last) nine bytes
        //  become padding, hence the offset.
        _dataOffset = 9;
      }
      else
      {
        _DcBcFcUdShiftRegSize = cTLC5951DcBcFcUdShiftRegSize;
        bufferSize = cTLC5951DcBcFcUdShiftRegSize * numberOfDevices;
      }

      _pwmChannelsPerDevice = cTLC5951PwmChannelsPerDevice;
      _pDcBcFcUdBuffer = (uint8_t*)malloc(bufferSize);
      memset(_pDcBcFcUdBuffer, 0, bufferSize);
      break;

    default:
      // _DcBcFcUdShiftRegSize is not used in this case, but initialize it anyway...
      _DcBcFcUdShiftRegSize = cTLC5947ShiftRegSize;
      _pwmChannelsPerDevice = cTLC5947PwmChannelsPerDevice;
      _pDcBcFcUdBuffer = nullptr;
  }

  bufferSize = cPwmBytesPerChannel * _pwmChannelsPerDevice * numberOfDevices;
  _pPwmBuffer = (uint16_t*)malloc(bufferSize);
  memset(_pPwmBuffer, 0, bufferSize);
}


void TLC59xx::setPwmChannel(const uint16_t channel, const uint16_t value)
{
  uint16_t modifiedChannel = _getModifiedChannel(channel);

  if (value <= cPwmMaximumValue)
  {
    _pPwmBuffer[modifiedChannel] = value;
  }
  else
  {
    _pPwmBuffer[modifiedChannel] = cPwmMaximumValue;
  }

  _gsHwRefreshRequired = true;
}


void TLC59xx::setPwmTriad(const uint16_t ledNumber, const uint16_t red, const uint16_t green, const uint16_t blue)
{
  uint16_t channel = (ledNumber * 3);

  // if calling this function, presumably one wants the order of red, green,
  //  and blue preserved...
  if ((_reversingMode == ReversingMode::NoReversing) || (_reversingMode == ReversePerDeviceAcrossBuffer))
  {
    setPwmChannel(channel++, blue);
    setPwmChannel(channel++, green);
    setPwmChannel(channel, red);
  }
  else
  {
    setPwmChannel(channel++, red);
    setPwmChannel(channel++, green);
    setPwmChannel(channel, blue);
  }
}


uint8_t TLC59xx::getDcChannel(const uint16_t channel)
{
  uint16_t modifiedChannel = _getModifiedChannel(channel),
           deviceNumber = modifiedChannel / _pwmChannelsPerDevice,
           startBit = (modifiedChannel - (deviceNumber * _pwmChannelsPerDevice)) * 7,
           firstByte = startBit / 8,
           firstBit = startBit % 8,
           scratch = 0;

  if (_pDcBcFcUdBuffer != nullptr)
  {
    scratch = _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte] << 8 |
              _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte + 1];
    scratch = scratch >> (9 - firstBit);

    scratch &= 0x7f;    // keep only seven bits
  }
  return scratch;
}


void TLC59xx::setDcChannel(const uint16_t channel, const uint8_t value)
{
  if (_pDcBcFcUdBuffer != nullptr)
  {
    uint16_t modifiedChannel = _getModifiedChannel(channel),
             deviceNumber = modifiedChannel / _pwmChannelsPerDevice,
             startBit = (modifiedChannel - (deviceNumber * _pwmChannelsPerDevice)) * 7,
             firstByte = startBit / 8,
             firstBit = startBit % 8,
             bitMask = 0b1111111000000000,
             scratch = 0,
             shiftedValue = value << 9;

    scratch = _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte] << 8 |
              _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte + 1];
    bitMask = ~(bitMask >> firstBit);
    shiftedValue = shiftedValue >> firstBit;

    scratch &= bitMask;       // mask off the seven bits we are going to update
    scratch |= shiftedValue;  // put the new bits/value into place

    if (firstByte < 20)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte] = scratch >> 8;
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte + 1] = scratch;
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951DcBaseIndex + _dataOffset + firstByte] = scratch >> 8;
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}


void TLC59xx::setDcTriad(const uint16_t ledNumber, const uint8_t red, const uint8_t green, const uint8_t blue)
{
  if (_pDcBcFcUdBuffer != nullptr)
  {
    uint16_t channel = ledNumber * 3;

    // if calling this function, presumably one wants the order of red, green,
    //  and blue preserved...
    if ((_reversingMode == ReversingMode::NoReversing) || (_reversingMode == ReversePerDeviceAcrossBuffer))
    {
      setDcChannel(channel++, blue);
      setDcChannel(channel++, green);
      setDcChannel(channel, red);
    }
    else
    {
      setDcChannel(channel++, red);
      setDcChannel(channel++, green);
      setDcChannel(channel, blue);
    }
  }
}


uint8_t TLC59xx::getBcRed(const uint8_t deviceNumber)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    return _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 2];
  }
  else
  {
    return 0;
  }
}


uint8_t TLC59xx::getBcGreen(const uint8_t deviceNumber)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    return _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 1];
  }
  else
  {
    return 0;
  }
}


uint8_t TLC59xx::getBcBlue(const uint8_t deviceNumber)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    return _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset];
  }
  else
  {
    return 0;
  }
}


void TLC59xx::setBcRed(const uint8_t deviceNumber, const uint8_t value)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 2] = value;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


void TLC59xx::setBcGreen(const uint8_t deviceNumber, const uint8_t value)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 1] = value;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


void TLC59xx::setBcBlue(const uint8_t deviceNumber, const uint8_t value)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset] = value;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


void TLC59xx::setBcTriad(const uint16_t deviceNumber, const uint8_t red, const uint8_t green, const uint8_t blue)
{
  if ((deviceNumber < _numberOfDevices) && (_pDcBcFcUdBuffer != nullptr))
  {
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 2] = red;
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset + 1] = green;
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951BcBaseIndex + _dataOffset] = blue;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


TLC59xx::tDcAdjRange TLC59xx::getDcRangeRed(const uint8_t deviceNumber)
{
  if ((_pDcBcFcUdBuffer != nullptr) &&
      ((_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951DcRangeRed)) == true))
  {
    return tDcAdjRange::DcRange33to100;
  }
  else
  {
    return tDcAdjRange::DcRange0to66;
  }
}


TLC59xx::tDcAdjRange TLC59xx::getDcRangeGreen(const uint8_t deviceNumber)
{
  if ((_pDcBcFcUdBuffer != nullptr) &&
      ((_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951DcRangeGreen)) == true))
  {
    return tDcAdjRange::DcRange33to100;
  }
  else
  {
    return tDcAdjRange::DcRange0to66;
  }
}


TLC59xx::tDcAdjRange TLC59xx::getDcRangeBlue(const uint8_t deviceNumber)
{
  if ((_pDcBcFcUdBuffer != nullptr) &&
      ((_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951DcRangeBlue)) == true))
  {
    return tDcAdjRange::DcRange33to100;
  }
  else
  {
    return tDcAdjRange::DcRange0to66;
  }
}


void TLC59xx::setDcRangeRed(const uint8_t deviceNumber, const DcAdjRange range)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if (range == DcAdjRange::DcRange33to100)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] |= (1 << TLC5951FcRegisterBits::TLC5951DcRangeRed);
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] &= ~(1 << TLC5951FcRegisterBits::TLC5951DcRangeRed);
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}

void TLC59xx::setDcRangeGreen(const uint8_t deviceNumber, const DcAdjRange range)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if (range == DcAdjRange::DcRange33to100)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] |= (1 << TLC5951FcRegisterBits::TLC5951DcRangeGreen);
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] &= ~(1 << TLC5951FcRegisterBits::TLC5951DcRangeGreen);
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}


void TLC59xx::setDcRangeBlue(const uint8_t deviceNumber, const DcAdjRange range)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if (range == DcAdjRange::DcRange33to100)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] |= (1 << TLC5951FcRegisterBits::TLC5951DcRangeBlue);
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] &= ~(1 << TLC5951FcRegisterBits::TLC5951DcRangeBlue);
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}


bool TLC59xx::autoRepeatIsEnabled(const uint8_t deviceNumber)
{
  if (_pDcBcFcUdBuffer != nullptr)
  {
    return _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951DispRepMode);
  }
  return false;
}


void TLC59xx::setAutoRepeatMode(const uint8_t deviceNumber, const bool autoRepeatEnabled)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if (autoRepeatEnabled == true)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] |= (1 << TLC5951FcRegisterBits::TLC5951DispRepMode);
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] &= ~(1 << TLC5951FcRegisterBits::TLC5951DispRepMode);
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}


bool TLC59xx::timingResetModeIsEnabled(const uint8_t deviceNumber)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    return _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951TimingResetMode);
  }
  return false;
}


void TLC59xx::setTimingResetMode(const uint8_t deviceNumber, const bool autoResetEnabled)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if (autoResetEnabled == true)
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] |= (1 << TLC5951FcRegisterBits::TLC5951TimingResetMode);
    }
    else
    {
      _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] &= ~(1 << TLC5951FcRegisterBits::TLC5951TimingResetMode);
    }

    _dcBcFcUdHwRefreshRequired = true;
  }
}


TLC59xx::tGreyscaleResolution TLC59xx::getPwmResolution(const uint8_t deviceNumber)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    if ((_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951GsResolutionBitmaskHigh)) == false)
    {
      return tGreyscaleResolution::Gs12BitMode;
    }
    else if ((_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] & (1 << TLC5951FcRegisterBits::TLC5951GsResolutionBitmaskLow)) == false)
    {
      return tGreyscaleResolution::Gs10BitMode;
    }
    else
    {
      return tGreyscaleResolution::Gs8BitMode;
    }
  }
  return tGreyscaleResolution::Gs12BitMode;
}


void TLC59xx::setPwmResolution(const uint8_t deviceNumber, const GreyscaleResolution resolution)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    uint8_t functionReg = _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset];

    functionReg &= ~(3 << TLC5951FcRegisterBits::TLC5951GsResolutionBitmaskLow);
    functionReg |= (resolution << TLC5951FcRegisterBits::TLC5951GsResolutionBitmaskLow);

    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951FcBaseIndex + _dataOffset] = functionReg;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


uint16_t TLC59xx::getUserData(const uint8_t deviceNumber)
{
  uint16_t word = 0;

  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    word = (_pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951UdBaseIndex + _dataOffset] << 8) |
            _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951UdBaseIndex + _dataOffset + 1];
  }
  return word;
}


void TLC59xx::setUserData(const uint8_t deviceNumber, const uint16_t data)
{
  if ((_pDcBcFcUdBuffer != nullptr) && (deviceNumber < _numberOfDevices))
  {
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951UdBaseIndex + _dataOffset] = (data >> 8);
    _pDcBcFcUdBuffer[(deviceNumber * _DcBcFcUdShiftRegSize) + cTLC5951UdBaseIndex + _dataOffset + 1] = data;

    _dcBcFcUdHwRefreshRequired = true;
  }
}


bool TLC59xx::isOutputOpen(const uint16_t channel)
{
  uint16_t deviceNumber = channel / _pwmChannelsPerDevice,
           offset = (deviceNumber * cTLC5951ShiftRegSize) + cTLC5951LodBaseIndex + (channel % 3);
  uint8_t  bit = (channel - (deviceNumber * _pwmChannelsPerDevice)) / 3;

  if ((channel < _pwmChannelsPerDevice * _numberOfDevices) &&
      ((_pPwmBuffer[offset] & (1 << bit)) == true))
  {
    return true;
  }
  return false;
}


bool TLC59xx::isOutputShorted(const uint16_t channel)
{
  uint16_t deviceNumber = channel / _pwmChannelsPerDevice,
           offset = (deviceNumber * cTLC5951ShiftRegSize) + cTLC5951LsdBaseIndex + (channel % 3);
  uint8_t  bit = (channel - (deviceNumber * _pwmChannelsPerDevice)) / 3;

  if ((channel < _pwmChannelsPerDevice * _numberOfDevices) &&
      ((_pPwmBuffer[offset] & (1 << bit)) == true))
  {
    return true;
  }
  return false;
}


bool TLC59xx::isThermalErrorActive(const uint8_t deviceNumber)
{
  if ((deviceNumber < _numberOfDevices) &&
      ((_pPwmBuffer[(deviceNumber * cTLC5951ShiftRegSize) + cTLC5951LodBaseIndex + 6] & (1 << 7)) == true))
  {
    return true;
  }
  return false;
}


TLC59xx::tReversingMode TLC59xx::getReversingMode()
{
  return _reversingMode;
}


void TLC59xx::setReversingMode(ReversingMode newReversingMode)
{
  _reversingMode = newReversingMode;
}


uint16_t* TLC59xx::getPwmBufferPtrForWrite()
{
  return _pPwmBuffer;
}


uint8_t* TLC59xx::getDcBcFcUdBufferPtrForWrite()
{
  return _pDcBcFcUdBuffer;
}


bool TLC59xx::gsHwRefreshRequired(const bool clearFlag)
{
  if ((clearFlag == true) && (_gsHwRefreshRequired == true))
  {
    _gsHwRefreshRequired = false;
    return true;
  }

  return _gsHwRefreshRequired;
}


bool TLC59xx::dcBcFcUdHwRefreshRequired(const bool clearFlag)
{
  if ((clearFlag == true) && (_dcBcFcUdHwRefreshRequired == true))
  {
    _dcBcFcUdHwRefreshRequired = false;
    return true;
  }

  return _dcBcFcUdHwRefreshRequired;
}


void TLC59xx::setGsHwRefreshRequired()
{
  _gsHwRefreshRequired = true;
}


void TLC59xx::setDcBcFcUdHwRefreshRequired()
{
  _dcBcFcUdHwRefreshRequired = true;
}


uint16_t TLC59xx::_getModifiedChannel(const uint16_t channel)
{
  uint16_t modifiedChannel = 0,
           deviceNumber = 0;

  if (channel < _pwmChannelsPerDevice * _numberOfDevices)
  {
    switch (_reversingMode)
    {
      case ReversePerDevice:
        deviceNumber = (channel / _pwmChannelsPerDevice);
        modifiedChannel = ((deviceNumber + 1) * _pwmChannelsPerDevice) - channel - 1;
        modifiedChannel += (deviceNumber * _pwmChannelsPerDevice);
        break;

      case ReverseAcrossBuffer:
        modifiedChannel = (_numberOfDevices * _pwmChannelsPerDevice) - channel - 1;
        break;

      case ReversePerDeviceAcrossBuffer:
        modifiedChannel = (_numberOfDevices * _pwmChannelsPerDevice) - channel - 1;
        deviceNumber = (modifiedChannel / _pwmChannelsPerDevice);
        modifiedChannel = ((deviceNumber + 1) * _pwmChannelsPerDevice) - modifiedChannel - 1;
        modifiedChannel += (deviceNumber * _pwmChannelsPerDevice);
        break;

      default:
        modifiedChannel = channel;
        break;
    }
  }
  return modifiedChannel;
}


}
