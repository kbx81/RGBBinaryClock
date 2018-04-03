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

// The number of 8-bit registers in the chip
//
static const uint8_t cNumberOfRegisters = 9;

// Registers of interest
//
static const uint8_t cRegisterPointer = 0x00;
static const uint8_t cConfigurationRegister = 0x01;
static const uint8_t cUpperLimitRegister = 0x02;
static const uint8_t cLowerLimitRegister = 0x03;
static const uint8_t cCriticalLimitRegister = 0x04;
static const uint8_t cTemperatureRegister = 0x05;
static const uint8_t cManufacturerIdRegister = 0x06;
static const uint8_t cDeviceIdRegister = 0x07;
static const uint8_t cResolutionRegister = 0x08;

// A full copy of MCP9808 registers, refreshed by calling refresh() functions
//
static uint16_t mcp9808Register[cNumberOfRegisters];

// Read and Write buffers
//
static uint16_t readBuffer[4];
static uint16_t writeBuffer[2];


bool isConnected()
{
  // Address the config register and read one byte
  while (Hardware::i2cTransfer(cChipAddress, &cManufacturerIdRegister, 1, (uint8_t*)readBuffer, 2) == false);
  while (Hardware::i2cIsBusy() == true);

  mcp9808Register[cManufacturerIdRegister] = readBuffer[0];

  // If the device ID is as expected, the IC is probably connected
  return (mcp9808Register[cManufacturerIdRegister] == cManufacturerId);
}


uint16_t getTemperatureRegister()
{
  return mcp9808Register[cTemperatureRegister];
}


int16_t getTemperatureWholePart()
{
  int16_t temperature = mcp9808Register[cTemperatureRegister];

  temperature = (temperature & 0x0ff0) >> 4;

  if ((mcp9808Register[cTemperatureRegister] & 0x1000) == true)
  {
    temperature *= -1;
  }

  return temperature;
}


uint16_t getTemperatureFractionalPart()
{
  return (mcp9808Register[cTemperatureRegister] & 0x000f);
}


bool refresh()
{
  // Address the first (temperature) register, then read all the registers
  return Hardware::i2cTransfer(cChipAddress, &cRegisterPointer, 1, (uint8_t*)mcp9808Register, cNumberOfRegisters * 2);
}


bool refreshTemp()
{
  // Address the temperature register
  return Hardware::i2cTransfer(cChipAddress, &cTemperatureRegister, 1, (uint8_t*)(mcp9808Register + cTemperatureRegister), 2);
}


}

}
