//
// kbx81's binary clock DS3234 RTC class
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
#include "DS3234.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace DS3234 {

// We add one to all of the regsiter values and the register count because, when
// transfering data to/from the chip, the first byte is always a byte indicating
// the register address at which the read/write operation should begin. Since
// SPI always transmits and receives simultaneously, this byte is required
// despite that it is essentially wasted in the receive buffer.

// The number of registers in the chip plus one
//
static const uint8_t cNumberOfRegisters = 19 + 1;

// Registers of interest
//
static const uint8_t cSecondsRegister = 0x00 + 1;
static const uint8_t cMinutesRegister = 0x01 + 1;
static const uint8_t cHoursRegister = 0x02 + 1;
static const uint8_t cDoWRegister = 0x03 + 1;
static const uint8_t cDateRegister = 0x04 + 1;
static const uint8_t cMonthRegister = 0x05 + 1;
static const uint8_t cYearRegister = 0x06 + 1;
// static const uint8_t cAlarm1SecondsRegister = 0x07 + 1;
// static const uint8_t cAlarm1MinutesRegister = 0x08 + 1;
// static const uint8_t cAlarm1HoursRegister = 0x09 + 1;
// static const uint8_t cAlarm1DoWDateRegister = 0x0a + 1;
// static const uint8_t cAlarm2MinutesRegister = 0x0b + 1;
// static const uint8_t cAlarm2HoursRegister = 0x0c + 1;
// static const uint8_t cAlarm2DoWDateRegister = 0x0d + 1;
static const uint8_t cControlRegister = 0x0e + 1;
static const uint8_t cStatusRegister = 0x0f + 1;
// static const uint8_t cAgingRegister = 0x10 + 1;
static const uint8_t cTemperatureMSBRegister = 0x11 + 1;
static const uint8_t cTemperatureLSBRegister = 0x12 + 1;
static const uint8_t cTemperatureConversionDisableRegister = 0x13 + 1;
static const uint8_t cSramAddressRegister = 0x18 + 1;
static const uint8_t cSramDataRegister = 0x19 + 1;

static const uint8_t cAddressByte = 0x00;

// A byte we use to test if the DS3234 is connected
//
static const uint8_t cTestByte = 0x5a;

// Setting this bit in the address byte makes the operation a write
//
static const uint8_t cWriteBit = 0x80;

// This bit indicates if the oscillator stopped, likely invalidating the time
//
static const uint8_t cOsfBit = (1 << 7);

// This bit indicates the century (in the month register)
//
static const uint8_t cCenturyBit = (1 << 7);


// The year base
//
static uint16_t gYearBase = 2000;

// A full copy of DS3234 registers, refreshed by calling refresh() functions
//
static uint8_t ds3234Register[cNumberOfRegisters];

// A spare buffer for when we don't need to read/write data
//
static uint8_t spareBuffer[cNumberOfRegisters];


// Function to convert BCD format into binary format
//
static inline uint8_t convertBcdToBin(const uint8_t bcd)
{
    return (bcd & 0xf) + ((bcd >> 4) * 10);
}


// Function to convert binary to BCD format
//
static inline uint8_t convertBinToBcd(const uint8_t bin)
{
    return (bin % 10) + ((bin / 10) << 4);
}


void setBaseYear(uint16_t yearBase)
{
    gYearBase = yearBase;
}


DateTime getDateTime()
{
  // Convert the values into a date object and return it
  return DateTime(
      static_cast<uint16_t>(convertBcdToBin(ds3234Register[cYearRegister])) + ((ds3234Register[cMonthRegister] & cCenturyBit) != 0 ? (gYearBase + 100) : gYearBase),
      convertBcdToBin(ds3234Register[cMonthRegister] & 0x1f),
      convertBcdToBin(ds3234Register[cDateRegister] & 0x3f),
      convertBcdToBin(ds3234Register[cHoursRegister] & 0x3f),
      convertBcdToBin(ds3234Register[cMinutesRegister] & 0x7f),
      convertBcdToBin(ds3234Register[cSecondsRegister] & 0x7f));
}


void setDateTime(const DateTime &dateTime)
{
  // we need to write the starting address first...
  ds3234Register[cAddressByte] = cAddressByte | cWriteBit;

  ds3234Register[cSecondsRegister] = dateTime.second(true);
  ds3234Register[cMinutesRegister] = dateTime.minute(true);
  ds3234Register[cHoursRegister] = dateTime.hour(true, false);
  ds3234Register[cDoWRegister] = dateTime.dayOfWeek();
  ds3234Register[cDateRegister] = dateTime.day(true);
  ds3234Register[cMonthRegister] = dateTime.month(true);
  ds3234Register[cYearRegister] = dateTime.yearShort(true);

  // Based on the buffer we set up above, we'll start at the seconds register
  //  and write up from there -- what we read back is irrelevant/discarded
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, spareBuffer + 4, ds3234Register, 8, false) == false);

  // again, we need to write the starting address first...
  spareBuffer[cAddressByte] = (cStatusRegister - 1) | cWriteBit;
  // clear the OSF bit 7, keep EN32kHz at its default
  spareBuffer[1] = 0x48;

  // Address the status register and write it to clear the OSF
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, spareBuffer + 4, spareBuffer, 2, false) == false);
}


bool isConnected()
{
  // Start writing at the SRAM Address register
  spareBuffer[cAddressByte] = (cSramAddressRegister - 1) | cWriteBit;
  // Set SRAM Address register to point to address zero
  spareBuffer[1] = cAddressByte;
  // We want to write the test byte as data
  spareBuffer[2] = cTestByte;
  // Set the SRAM Address regsiter
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, ds3234Register, spareBuffer, 3, false) == false);

  // Start writing at the SRAM Address register
  spareBuffer[cAddressByte] = (cSramAddressRegister - 1) | cWriteBit;
  // Set SRAM Address register to point to address zero
  spareBuffer[1] = cAddressByte;
  // Reset the SRAM Address regsiter
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, ds3234Register, spareBuffer, 2, false) == false);

  // This is the address we want to start reading from
  spareBuffer[cAddressByte] = cSramDataRegister - 1;
  // Try to read the byte back and check if it matches what we expect
  while (Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, ds3234Register, spareBuffer, 2, false) == false);

  // If we read back the byte we wrote, the RTC is very likely connected
  if (ds3234Register[1] == cTestByte)
  {
    // Kick off a full register refresh
    while (refresh() == false);

    return true;
  }

  return false;
}


bool isRunning()
{
  // If the 7th bit is zero, the RTC is running
  return (ds3234Register[cControlRegister] & cOsfBit) == 0;
}


bool isValid()
{
  // If the 7th bit (OSF) is zero, the oscillator has not stopped so the RTC is valid
  return (ds3234Register[cStatusRegister] & cOsfBit) == 0;
}


int16_t getTemperature()
{
  return (ds3234Register[cTemperatureMSBRegister] << 8) | ds3234Register[cTemperatureLSBRegister];
}


bool refresh()
{
  // This is the address we want to start reading from
  spareBuffer[cAddressByte] = cAddressByte;
  // Address the seconds register, then (try to) read all the registers
  return Hardware::spiTransfer(Hardware::SpiPeripheral::Rtc, ds3234Register, spareBuffer, cNumberOfRegisters, false);
}


}

}
