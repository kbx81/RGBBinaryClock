//
// kbx81's binary clock DMX-512 controller class
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

namespace Dmx512Controller {


// Initialize it all
//
void initialize();

// Controls main application and views based on DMX-512 input; sets strobe delay
//
void controller();

// Should be called by a timer (systick) once every millisecond (ideally)
//
void strobeTimer();


}

}
