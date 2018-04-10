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
#pragma once

#include <cstdint>
#include "DateTime.h"


namespace kbxBinaryClock {

namespace DS3231 {


// Initialize the real time clock driver.
//
// @param yearBase The year base which is used for the RTC.
//    The RTC stores the year only with two digits, plus one
//    additional bit for the next century. If you set the
//    year base to 2000, the RTC will hold the correct time
//    for 200 years, starting from 2000-01-01 00:00:00.
//
void setBaseYear(uint16_t yearBase = 2000);

// Get the current date/time
//
DateTime getDateTime();

// Set the date/time
//
void setDateTime(const DateTime &dateTime);

// Check if the RTC is connected
//
bool isConnected();

// Check if the RTC is running
//
bool isRunning();

// Check if the RTC is valid
//
bool isValid();

// Enable or disable the 32kHz output signal
//
void set32kHzOut(const bool isEnabled);

// Get the raw temperature register value
//
uint16_t getTemperatureRegister();

// Get the whole-number part of the temperature register (in degrees celsius)
//
int16_t getTemperatureWholePart();

// Get the fractional part of the temperature register (in degrees celsius)
//
uint16_t getTemperatureFractionalPart();

// Read all registers from the DS3231
//  Returns result of read from Hardware::i2cTransfer()
bool refresh();

// Read the time and temperature registers from the DS3231
//  Returns result of read from Hardware::i2cTransfer()
bool refreshTimeDateTemp();

// Read the time and date registers from the DS3231
//  Returns result of read from Hardware::i2cTransfer()
bool refreshTimeDate();

}

}
