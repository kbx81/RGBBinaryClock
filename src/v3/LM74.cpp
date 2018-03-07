//
// kbx81's binary clock LM75 temperature sensor class
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
#include <stdio.h>
#include "DateTime.h"
#include "LM74.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace LM74 {


// The chip's address on the I2C bus
//
static const uint8_t cChipAddress = 0x48;

// The number of 8-bit registers in the chip
//
static const uint8_t cNumberOfRegisters = 7;

// Registers of interest
//
static const uint8_t cTemperatureRegister = 0x00;   // two bytes
static const uint8_t cConfigurationRegister = 0x01; // one byte
static const uint8_t cThystRegister = 0x02;         // two bytes
static const uint8_t cTosRegister = 0x03;           // two bytes

// A full copy of DS3231 registers, refreshed by calling refresh() functions
//
static uint8_t lm75Register[cNumberOfRegisters];

// Read and Write buffers
//
static uint8_t readBuffer[4];
static uint8_t writeBuffer[2];


bool isConnected()
{
  // Address the config register and read one byte
  // uint8_t result = Hardware::i2c_transfer7(I2C1, cChipAddress, &cConfigurationRegister, 1, readBuffer, 1);

  lm75Register[cConfigurationRegister + 1] = readBuffer[0];

  // If bits 4 through 6 are zero and there wasn't a timeout, the IC is probably connected
  return (((lm75Register[cConfigurationRegister + 1] & 0xe0) == 0)); // & (result == 0));
}


int16_t getTemperature()
{
  return (lm75Register[cTemperatureRegister] << 8) | lm75Register[cTemperatureRegister + 1];
}


uint8_t refresh()
{
  // Address the first (temperature) register, then read all the registers
  // return Hardware::i2c_transfer7(I2C1, cChipAddress, &cTemperatureRegister, 1, lm75Register, cNumberOfRegisters);
}


uint8_t refreshTemp()
{
  // Address the temperature register
  // return Hardware::i2c_transfer7(I2C1, cChipAddress, &cTemperatureRegister, 1, lm75Register, 2);
}


}

}
