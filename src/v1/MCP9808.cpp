//
// kbx81's binary clock MCP9808 temperature sensor class
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
#include "DateTime.h"
#include "MCP9808.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace MCP9808 {


// The chip's address on the I2C bus
//
static const uint8_t cChipAddress = 0x18;

// The chip's manufacturer ID
//
static const uint16_t cManufacturerId = 0x0054;

// The number of 16-bit registers in the chip
//
static const uint8_t cNumberOfRegisters = 9;

// Size of registers in bytes
//
static const uint8_t cRegisterSizeInBytes = 2;

// How long we'll wait for I2C acknowledgement during isConnected()
//
static const uint16_t cI2cTimeout = 1000;

// Registers of interest
//
static const uint8_t cRegisterPointer = 0x00;
// static const uint8_t cConfigurationRegister = 0x01;
// static const uint8_t cUpperLimitRegister = 0x02;
// static const uint8_t cLowerLimitRegister = 0x03;
// static const uint8_t cCriticalLimitRegister = 0x04;
static const uint8_t cTemperatureRegister = 0x05;
static const uint8_t cManufacturerIdRegister = 0x06;
// static const uint8_t cDeviceIdRegister = 0x07;
// static const uint8_t cResolutionRegister = 0x08;

// A full copy of MCP9808 registers, refreshed by calling refresh() functions
//
static uint8_t mcp9808Register[cNumberOfRegisters * cRegisterSizeInBytes];

// Read and Write buffers
//
static uint8_t readBuffer[cNumberOfRegisters * cRegisterSizeInBytes];
// static uint16_t writeBuffer[2];


bool isConnected()
{
  uint16_t timeout = cI2cTimeout, manufacturerId = 0;
  // Address the config register and read one byte
  while (Hardware::i2cTransfer(cChipAddress, &cManufacturerIdRegister, 1, mcp9808Register + (cManufacturerIdRegister * cRegisterSizeInBytes), 2) == false);
  while ((Hardware::i2cIsBusy() == true) && (--timeout > 0));

  if (timeout > 0)
  {
    manufacturerId = (mcp9808Register[cManufacturerIdRegister * cRegisterSizeInBytes] << 8) | mcp9808Register[(cManufacturerIdRegister * cRegisterSizeInBytes) + 1];
  }
  else
  {
    Hardware::i2cAbort();
  }
  // If the device ID is as expected, the IC is probably connected
  return (manufacturerId == cManufacturerId);
}


uint16_t getTemperatureRegister()
{
  return (mcp9808Register[cTemperatureRegister * cRegisterSizeInBytes] << 8) | mcp9808Register[(cTemperatureRegister * cRegisterSizeInBytes) + 1];
}


int16_t getTemperatureWholePart()
{
  int16_t temperature = (mcp9808Register[cTemperatureRegister * cRegisterSizeInBytes] << 8) | mcp9808Register[(cTemperatureRegister * cRegisterSizeInBytes) + 1];

  temperature = (temperature & 0x0ff0) >> 4;

  if ((mcp9808Register[cTemperatureRegister * cRegisterSizeInBytes] & 0x10) == true)
  {
    temperature *= -1;
  }

  return temperature;
}


uint16_t getTemperatureFractionalPart()
{
  return (mcp9808Register[(cTemperatureRegister * cRegisterSizeInBytes) + 1] & 0x0f);
}


bool refresh()
{
  // Address the first (temperature) register, then read all the registers
  return Hardware::i2cTransfer(cChipAddress, &cRegisterPointer, 1, mcp9808Register, cNumberOfRegisters * cRegisterSizeInBytes);
}


bool refreshTemp()
{
  // Address the temperature register
  return Hardware::i2cTransfer(cChipAddress, &cTemperatureRegister, 1, mcp9808Register + (cTemperatureRegister * cRegisterSizeInBytes), 2);
}


}

}
