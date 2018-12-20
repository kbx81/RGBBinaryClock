//
// kbx81's binary clock DisplayManager Library
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

#include "Display.h"
#include "RgbLed.h"


namespace kbxBinaryClock {

/// @brief kbx Binary Clock DisplayManager
///

namespace DisplayManager
{
  /// @brief Initialize DisplayManager
  ///
  void initialize();

  /// @brief Refreshes drivers and intensities of LEDs that are changing intensity levels due to (an) active crossfade(s)
  ///
  void refresh();

  /// @brief Triggers a refresh of LED intensities next time refresh() is called
  ///
  void tick();

  /// @brief Enables/Disables control of the status LED by refresh() (above)
  /// @param New state for status LED auto-refreshing
  ///
  void setStatusLedAutoRefreshing(const bool autoRefreshEnabled);

  /// @brief Returns the current "master" intensities of the display
  ///
  uint16_t getMasterIntensity();

  /// @brief Sets the "master" intensity of the display, where 10000 = 100%.
  ///   Values are gamma-corrected.
  /// @param New master intensity for display. Display is updated at next refresh()
  void setMasterIntensity(const uint16_t intensity);

  /// @brief Get the state of display blanking
  /// @return true if display is blank/off
  ///
  bool getDisplayBlanking();

  /// @brief Blank the display
  /// @param display is blank (off) if true
  ///
  void setDisplayBlanking(const bool blank);

  /// @brief Set the dot correction range used by the LED drivers
  /// @param drivers will use the upper (33% to 100%) range if true
  ///
  void setDotCorrectionRange(const bool useUpperRange);

  /// @brief Set the dot correction value for a single LED
  /// @param LED number to adjust
  /// @param adjustment level for given LED (7 bits)
  ///
  void setDotCorrectionValue(const uint8_t ledNumber, const uint8_t value);

  /// @brief Causes the display to blink twice (acknowledgement of some action)
  ///
  void doubleBlink();

  /// @brief Writes the passed display into the display buffer. LEDs will fade to the
  ///  intensities in the new display at the specified rates.
  /// @param display to be written
  /// @param status LED state
  ///
  void writeDisplay(const Display &display);
  void writeDisplay(const Display &display, const RgbLed &statusLed);


}


}
