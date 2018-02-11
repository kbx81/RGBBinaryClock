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

#include "RgbLed.h"

/// @mainpage
///
/// @section intro_sec Introduction
///
/// This library contains a class to manage the Binary Clock's RGB LEDs.
///
/// @section requirements_sec Requirements
///
/// This library is written in a manner so as to be compatible on a range of
/// CPUs/MCUs. It has been tested on Arduino and STM32F0 platforms. It requires
/// a modern C++ compiler (C++11).
///
/// @section classes_sec Classes
///
/// There is only the RgbLed::RgbLed class. Read the documentation of this class
/// for all details.
///


namespace kbxBinaryClock {


  RgbLed::RgbLed(const uint16_t red, const uint16_t green, const uint16_t blue, const uint16_t rate)
    : _red(red),
      _green(green),
      _blue(blue),
      _rate(rate)
  {
  }


  void RgbLed::setRed(const uint16_t intensityRed)
  {
    _red = intensityRed;
  }


  void RgbLed::setGreen(const uint16_t intensityGreen)
  {
    _green = intensityGreen;
  }


  void RgbLed::setBlue(const uint16_t intensityBlue)
  {
    _blue = intensityBlue;
  }


  void RgbLed::setRGB(const uint16_t intensityRed, const uint16_t intensityGreen, const uint16_t intensityBlue)
  {
    _red = intensityRed;
    _green = intensityGreen;
    _blue = intensityBlue;
  }


  void RgbLed::setRate(const uint16_t rate)
  {
    _rate = rate;
  }


  uint16_t RgbLed::getRed() const
  {
    return _red;
  }


  uint16_t RgbLed::getGreen() const
  {
    return _green;
  }


  uint16_t RgbLed::getBlue() const
  {
    return _blue;
  }


  uint16_t RgbLed::getRate() const
  {
    return _rate;
  }


}
