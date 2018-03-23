//
// kbx81's binary clock DS3231 RTC class
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
#include <libopencm3/stm32/i2c.h>
#include "DateTime.h"
#include "DS3231.h"
#include "Hardware.h"


namespace kbxBinaryClock {

namespace DS3231 {


// The chip's address on the I2C bus
//
static const uint8_t cChipAddress = 0x68;

// The number of registers in the chip
//
static const uint8_t cNumberOfRegisters = 19;

// Registers of interest
//
static const uint8_t cSecondsRegister = 0x00;
static const uint8_t cMinutesRegister = 0x01;
static const uint8_t cHoursRegister = 0x02;
static const uint8_t cDoWRegister = 0x03;
static const uint8_t cDateRegister = 0x04;
static const uint8_t cMonthRegister = 0x05;
static const uint8_t cYearRegister = 0x06;
// static const uint8_t cAlarm1SecondsRegister = 0x07;
// static const uint8_t cAlarm1MinutesRegister = 0x08;
// static const uint8_t cAlarm1HoursRegister = 0x09;
// static const uint8_t cAlarm1DoWDateRegister = 0x0a;
// static const uint8_t cAlarm2MinutesRegister = 0x0b;
// static const uint8_t cAlarm2HoursRegister = 0x0c;
// static const uint8_t cAlarm2DoWDateRegister = 0x0d;
static const uint8_t cControlRegister = 0x0e;
static const uint8_t cStatusRegister = 0x0f;
// static const uint8_t cAgingRegister = 0x10;
static const uint8_t cTemperatureMSBRegister = 0x11;
static const uint8_t cTemperatureLSBRegister = 0x12;

// The year base
//
static uint16_t gYearBase = 2000;

// A full copy of DS3231 registers, refreshed by calling refresh() functions
//
static uint8_t ds3231Register[cNumberOfRegisters];

// Read and Write buffers
//
static uint8_t readBuffer[9];
static uint8_t writeBuffer[8];


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
      static_cast<uint16_t>(convertBcdToBin(ds3231Register[cYearRegister])) + ((ds3231Register[cMonthRegister] & (1 << 7)) != 0 ? (gYearBase + 100) : gYearBase),
      convertBcdToBin(ds3231Register[cMonthRegister] & 0x1f),
      convertBcdToBin(ds3231Register[cDateRegister] & 0x3f),
      convertBcdToBin(ds3231Register[cHoursRegister] & 0x3f),
      convertBcdToBin(ds3231Register[cMinutesRegister] & 0x7f),
      convertBcdToBin(ds3231Register[cSecondsRegister] & 0x7f));
}


void setDateTime(const DateTime &dateTime)
{
  // we need to write the starting address first...
  writeBuffer[0] = cSecondsRegister;
  // ...so all of the offsets below must be incremented by one
  writeBuffer[cSecondsRegister + 1] = dateTime.second(true);
  writeBuffer[cMinutesRegister + 1] = dateTime.minute(true);
  writeBuffer[cHoursRegister + 1] = dateTime.hour(true, false);
  writeBuffer[cDoWRegister + 1] = dateTime.dayOfWeek();
  writeBuffer[cDateRegister + 1] = dateTime.day(true);
  writeBuffer[cMonthRegister + 1] = dateTime.month(true);
  writeBuffer[cYearRegister + 1] = dateTime.yearShort(true);

  // Based on the buffer we set up above, we'll start at the seconds register
  //  and write up from there -- we won't read anything back
  Hardware::i2c_transfer7(I2C1, cChipAddress, writeBuffer, 8, readBuffer, 0);

  // we need to write the starting address first...
  writeBuffer[0] = cStatusRegister;
  // clear the OSF bit 7
  writeBuffer[1] = 0x04;

  // Address the status register and write it to clear the OSF
  Hardware::i2c_transfer7(I2C1, cChipAddress, writeBuffer, 2, readBuffer, 0);
}


bool isConnected()
{
  // Address the status register and read one byte
  uint8_t result = Hardware::i2c_transfer7(I2C1, cChipAddress, &cControlRegister, 1, readBuffer, 2);

  ds3231Register[cControlRegister] = readBuffer[0];
  ds3231Register[cStatusRegister] = readBuffer[1];

  // If bits 4 through 6 are zero and there wasn't a timeout, the IC is probably connected
  return (((ds3231Register[cStatusRegister] & 0x70) == 0) && (result == 0));
}


bool isRunning()
{
  // If the 7th bit is zero, the RTC is running
  return (ds3231Register[cControlRegister] & (1 << 7)) == 0;
}


bool isValid()
{
  // If the 7th bit (OSF) is zero, the oscillator has not stopped so the RTC is valid
  return (ds3231Register[cStatusRegister] & (1 << 7)) == 0;
}


uint16_t getTemperatureRegister()
{
  return (ds3231Register[cTemperatureMSBRegister] << 8) | ds3231Register[cTemperatureLSBRegister];
}


int16_t getTemperatureWholePart()
{
  return ((int8_t)ds3231Register[cTemperatureMSBRegister]);
}


uint16_t getTemperatureFractionalPart()
{
  return (ds3231Register[cTemperatureLSBRegister] >> 4);
}


uint8_t refresh()
{
  // Address the seconds register, then read all the registers
  return Hardware::i2c_transfer7(I2C1, cChipAddress, &cSecondsRegister, 1, ds3231Register, cNumberOfRegisters);
}


uint8_t refreshTimeDateTemp()
{
  // Address the seconds register, then read all the registers
  uint8_t result = Hardware::i2c_transfer7(I2C1, cChipAddress, &cTemperatureMSBRegister, 1, readBuffer, 9);

  ds3231Register[cTemperatureMSBRegister] = readBuffer[0];
  ds3231Register[cTemperatureLSBRegister] = readBuffer[1];
  ds3231Register[cSecondsRegister] = readBuffer[2];
  ds3231Register[cMinutesRegister] = readBuffer[3];
  ds3231Register[cHoursRegister] = readBuffer[4];
  ds3231Register[cDoWRegister] = readBuffer[5];
  ds3231Register[cDateRegister] = readBuffer[6];
  ds3231Register[cMonthRegister] = readBuffer[7];
  ds3231Register[cYearRegister] = readBuffer[8];

  return result;
}


uint8_t refreshTimeDate()
{
  // Address the seconds register, then read all the registers
  return Hardware::i2c_transfer7(I2C1, cChipAddress, &cSecondsRegister, 1, ds3231Register, 7);
}


}

}
