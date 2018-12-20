//
// kbx81's binary clock alarm handler
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

#include "Keys.h"
#include "Settings.h"


namespace kbxBinaryClock {

namespace AlarmHandler {


  /// @brief Initialize the alarms
  ///
  void initialize();

  /// @brief Handles key presses (during an active alarm)
  /// @param key pressed
  ///
  void keyHandler(Keys::Key key);

  /// @brief Called from the main application loop
  ///
  void loop();

  /// @brief Indicates whether or or not an alarm is active now
  /// @return true if one or more alarm(s) is/are active
  ///
  bool isAlarmActive();

  /// @brief Activates the latching alarm
  ///
  void activateLatchingAlarm();

  /// @brief Activates the momentary alarm
  ///
  void activateMomentaryAlarm();

  /// @brief Clears any active alarms
  ///
  void clearAlarm();

  /// @brief Deactivates the momentary alarm
  ///
  void releaseMomentaryAlarm();

  /// @brief Set new alarm settings
  /// @param settings new settings object with updated alarm settings
  ///
  void setSettings(Settings settings);
}

}
