//
// kbx81's binary clock DMX-512 receiver class
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


namespace kbxBinaryClock {

namespace DMX512Rx {


// Initialize it all
//
void initialize();

// Returns the level of the specified DMX-512 channel
//  Valid channels are 0 to 511
uint8_t channel(const uint16_t channel);

// Returns the start code of the current DMX-512 packet
//
uint8_t startCode();

// Returns true if a DM-512 signal is being received
//
bool signalIsActive();

// Interrupt service routines
//
void rxIsr();
void rxCompleteIsr();
void timerUartIsr();
void timerSupervisorIsr();

}

}
