//
// kbx81's binary clock DS1722 temperature sensor class
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
#include "DS1722.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace DS1722 {


// We add one to all of the regsiter values and the register count because, when
// transfering data to/from the chip, the first byte is always a byte indicating
// the register address at which the read/write operation should begin. Since
// SPI always transmits and receives simultaneously, this byte is required
// despite that it is essentially wasted in the receive buffer.

// The number of registers in the chip plus one
//
static const uint8_t cNumberOfRegisters = 3 + 1;

// Registers of interest
//
static const uint8_t cConfigurationRegister  = 0x00 + 1;
static const uint8_t cTemperatureLSBRegister = 0x01 + 1;
static const uint8_t cTemperatureMSBRegister = 0x02 + 1;

static const uint8_t cAddressByte = 0x00;

// What we want in the config register
//
static const uint8_t cConfigByte = 0xe4;

// Setting this bit in the address byte makes the operation a write
//
static const uint8_t cWriteBit = 0x80;

// A full copy of DS1722 registers, refreshed by calling refresh() functions
//
static uint8_t ds1722Register[cNumberOfRegisters];

// A spare buffer for when we don't need to read/write data
//
static uint8_t spareBuffer[cNumberOfRegisters];


bool isConnected()
{
  // Start writing at the Configuration register
  spareBuffer[cAddressByte] = (cConfigurationRegister - 1) | cWriteBit;
  // Set the config register to the value we want to use
  spareBuffer[1] = cConfigByte;
  // Write to the regsiter
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::TempSensor, ds1722Register, spareBuffer, 2, false) == false);
  while (Hardware::spiIsBusy() == true);

  // This is the address we want to start reading from
  spareBuffer[cAddressByte] = cConfigurationRegister - 1;
  // Try to read the byte (and other registers) back
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::TempSensor, ds1722Register, spareBuffer, 4, false) == false);
  while (Hardware::spiIsBusy() == true);

  // If we read back the byte we wrote, the IC is very likely connected
  if (ds1722Register[cConfigurationRegister] == cConfigByte)
  {
    // Kick off a full register refresh
    while (refresh() == false);

    return true;
  }

  return false;
}


int16_t getTemperature()
{
  return (ds1722Register[cTemperatureMSBRegister] << 8) | ds1722Register[cTemperatureLSBRegister + 1];
}


bool refresh()
{
  // This is the address we want to start reading from
  spareBuffer[cAddressByte] = cConfigurationRegister - 1;
  // Try to read the byte (and other registers) back
  return Hardware::spiTransfer(Hardware::SpiPeripheral::TempSensor, ds1722Register, spareBuffer, 4, false);
}


}

}
