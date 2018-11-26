//
// kbx81's binary clock Display Library
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

#include "RgbLed.h"


namespace kbxBinaryClock {

/// @brief kbx Binary Clock Display class
///
/// The binary clock's display class
///

class Display
{
public:
  /// @brief Default constructor
  ///
  Display();

  /// @brief Create a new display instance from raw values
  ///
  /// @param data Pointer to 24 RgbLed objects to be used to initialize the display's values
  ///
  Display(const RgbLed &color0, const RgbLed &color1, const RgbLed *data);

  /// @brief Create a new display instance from bitmap
  ///
  /// @param color0 Display color 0
  /// @param color1 Display color 1
  /// @param bitmap Bitmap indicating initial LED states (0 = color1, 1 = color1)
  ///
  Display(const RgbLed &color0, const RgbLed &color1, const uint32_t bitmap);

  /// @brief Create a new display instance from three bytes
  ///
  /// @param color0 Display color 0
  /// @param color1 Display color 1
  /// @param byte0 LSB to be displayed
  /// @param byte1 ... to be displayed
  /// @param byte2 MSB to be displayed
  ///
  Display(const RgbLed &color0, const RgbLed &color1, const uint8_t byte2, const uint8_t byte1, const uint8_t byte0);

public:
  /// Important constants
  ///
  static const uint8_t cPixelCount = 24;
  static const uint8_t cLedCount = cPixelCount * 3;
  static const uint16_t cLedMaxIntensity = 4095;

  /// Compare this Display to another
  ///
  bool operator==(const Display &other) const;
  bool operator!=(const Display &other) const;

  /// @brief Set this display's foreground color
  ///
  /// @param foreground Display foreground color
  ///
  void setDisplayColor0(const RgbLed &color);

  /// @brief Set this display's background color
  ///
  /// @param background Display background color
  ///
  void setDisplayColor1(const RgbLed &color);

  /// @brief Set this display's values from raw data
  ///
  /// @param data Pointer to 24 RgbLed objects used to set this display's state
  ///
  void setDisplayFromRaw(const RgbLed *data);

  /// @brief Set this display's state from a bitmap
  ///
  /// @param bitmap Bitmap containing LED states (1 = color1, 0 = color0)
  ///
  void setDisplayFromBitmap(const uint32_t bitmap);

  /// @brief Create a new display instance from three bytes
  ///
  /// @param byte0 LSB to be displayed
  /// @param byte1 ... to be displayed
  /// @param byte2 MSB to be displayed
  ///
  void setDisplayFromBytes(const uint8_t byte2, const uint8_t byte1, const uint8_t byte0);

  /// @brief Create a new display instance from six nibbles
  ///
  /// @param byte0 LSn to be displayed
  /// @param byte1 ... to be displayed
  /// @param byte2 ... to be displayed
  /// @param byte3 ... to be displayed
  /// @param byte4 ... to be displayed
  /// @param byte5 MSn to be displayed
  ///
  void setDisplayFromNibbles(const uint8_t byte5, const uint8_t byte4, const uint8_t byte3, const uint8_t byte2, const uint8_t byte1, const uint8_t byte0);

  /// @brief Set a single pixel's state
  ///
  /// @param pixelNumber Number of pixel to set (0 through 23)
  /// @param state pixel's color state (1 = color1, 0 = color0)
  ///
  void setPixelFromState(const uint8_t pixelNumber, const bool pixelState);

  /// @brief Set a single pixel's color
  ///
  /// @param pixelNumber Number of pixel to set (0 through 23)
  /// @param color New color for pixel
  ///
  void setPixelFromRaw(const uint8_t pixelNumber, const RgbLed &color);

  /// @brief Set a single pixel to be off
  ///
  /// @param pixelNumber Number of pixel to turn off (0 through 23)
  ///
  void setPixelOff(const uint8_t pixelNumber);

  /// @brief Set a number pixels to be off
  ///
  /// @param bitmap Bitmap of pixels to be turned off
  ///
  void setPixelsOff(const uint32_t bitmap);

  /// @brief Get a single pixel's state
  ///
  /// @param pixelNumber Number of pixel to set (0 through 23)
  /// @return State of pixel (1 = foreground, 0 = background)
  ///
  bool getPixelState(const uint8_t pixelNumber) const;

  /// @brief Get a single pixel's mask
  ///
  /// @param pixelNumber Number of pixel to set (0 through 23)
  /// @return State of pixel's mask
  ///
  bool getPixelMask(const uint8_t pixelNumber) const;

  /// @brief Get a single pixel's raw color
  ///
  /// @param pixelNumber Number of pixel to set (0 through 23)
  /// @return Pointer to RgbLed with color data from specified pixel
  ///
  RgbLed getPixelRaw(const uint8_t pixelNumber) const;


private:
  RgbLed _color0;       ///< color 0 (as used by bitmap)
  RgbLed _color1;       ///< color 1 (as used by bitmap)
  RgbLed _display[cPixelCount]; ///< values currently active on display
  uint32_t _displayBitmap;      ///< current display bitmap
  uint32_t _displayBitmapMask;  ///< 1 = set by bitmap, 0 = set to raw color
};


}
