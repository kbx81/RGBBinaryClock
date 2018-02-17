//
// kbx81's RGB LED Library
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

/// @brief kbx Binary Clock Main Application
///
/// The main application class
///

class RgbLed
{
public:
  /// @brief Create a new RgbLed instance with specified intensities and rate
  ///
  /// @param red Initial value for red element
  /// @param green Initial value for green element
  /// @param blue Initial value for blue element
  /// @param rate Initial (fade) rate for this LED
  ///
  RgbLed(const uint16_t red = 0, const uint16_t green = 0, const uint16_t blue = 0, const uint16_t rate = 0);

public:
  void setRed(const uint16_t intensity);
  void setGreen(const uint16_t intensity);
  void setBlue(const uint16_t intensity);
  void setRGB(const uint16_t intensityRed, const uint16_t intensityGreen, const uint16_t intensityBlue);
  void setRate(const uint16_t rate);

  uint16_t getRed() const;
  uint16_t getGreen() const;
  uint16_t getBlue() const;
  uint16_t getRate() const;

private:
  uint16_t _red, _green, _blue, _rate;  ///< Intensities for Red, Green, and Blue elements and Rate
};

}