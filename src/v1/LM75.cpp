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
#include "DateTime.h"
#include "LM75.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace LM75 {


// The chip's address on the I2C bus
//
static const uint8_t cChipAddress = 0x48;

// The number of 8-bit registers in the chip
//
static const uint8_t cNumberOfRegisters = 7;

// How long we'll wait for I2C acknowledgement during isConnected()
//
static const uint16_t cI2cTimeout = 1000;

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
// static uint8_t writeBuffer[2];


bool isConnected()
{
  uint16_t timeout = cI2cTimeout;
  lm75Register[cConfigurationRegister + 1] = 0xff;  // foolproofing
  // Address the config register and read one byte
  while (Hardware::i2cTransfer(cChipAddress, &cConfigurationRegister, 1, readBuffer, 1) == false);
  while ((Hardware::i2cIsBusy() == true) && (--timeout > 0));

  if (timeout > 0)
  {
    lm75Register[cConfigurationRegister + 1] = readBuffer[0];
  }
  else
  {
    Hardware::i2cAbort();
  }

  // If bits 4 through 6 are zero and there wasn't a timeout, the IC is probably connected
  return (((lm75Register[cConfigurationRegister + 1] & 0xe0) == 0));
}


uint16_t getTemperatureRegister()
{
  return (lm75Register[cTemperatureRegister] << 8) | lm75Register[cTemperatureRegister + 1];
}


int16_t getTemperatureWholePart()
{
  return ((int8_t)lm75Register[cTemperatureRegister]);
}


uint16_t getTemperatureFractionalPart()
{
  return ((lm75Register[cTemperatureRegister + 1] & 0x80) >> 4);
}


bool refresh()
{
  // Address the first (temperature) register, then read all the registers
  return Hardware::i2cTransfer(cChipAddress, &cTemperatureRegister, 1, lm75Register, cNumberOfRegisters);
}


bool refreshTemp()
{
  // Address the temperature register
  return Hardware::i2cTransfer(cChipAddress, &cTemperatureRegister, 1, lm75Register, 2);
}


}

}
