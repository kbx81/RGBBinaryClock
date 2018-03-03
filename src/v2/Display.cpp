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
#include "Display.h"

/// @mainpage
///
/// @section intro_sec Introduction
///
/// This library contains a class to manage the Binary Clock's RGB LED display.
///
/// @section requirements_sec Requirements
///
/// This library is written in a manner so as to be compatible on a range of
/// CPUs/MCUs. It has been tested on Arduino and STM32F0 platforms. It requires
/// a modern C++ compiler (C++11).
///
/// @section classes_sec Classes
///
/// There is only the Display::Display class. Read the documentation of this class
/// for all details.
///

namespace kbxBinaryClock {


// const uint8_t Display::cLedCount = 24;
// const uint16_t Display::cLedMaxIntensity = 4095;


Display::Display()
: _color0(0, 128, 0),
  _color1(128, 0, 0),
  _displayBitmap(0x000000),
  _displayBitmapMask(0xffffff)
{
  setDisplayFromBitmap(0);
}


Display::Display(const RgbLed &color0, const RgbLed &color1, const RgbLed *data)
: _color0(color0),
  _color1(color1),
  _displayBitmap(0x000000),
  _displayBitmapMask(0xffffff)
{
  uint8_t i = 0;

  for (i = 0; i < cLedCount; i++)
  {
    _display[i] = data[i];
  }
  _displayBitmapMask = 0x000000;
}


Display::Display(const RgbLed &color0, const RgbLed &color1, const uint32_t bitmap)
: _color0(color0),
  _color1(color1),
  _displayBitmap(bitmap),
  _displayBitmapMask(0xffffff)
{
  uint8_t i = 0;

  for (i = 0; i < cLedCount; i++)
  {
    if ((bitmap >> i) & 1)
    {
      _display[i] = color1;
    }
    else
    {
      _display[i] = color0;
    }
  }
}


Display::Display(const RgbLed &color0, const RgbLed &color1, const uint8_t byte2, const uint8_t byte1, const uint8_t byte0)
: _color0(color0),
  _color1(color1),
  _displayBitmap(byte0),
  _displayBitmapMask(0xffffff)
{
  uint8_t  i = 0;

  _displayBitmap |= (((uint32_t)byte2 << 16 ) | ((uint32_t)byte1 << 8));

  for (i = 0; i < cLedCount; i++)
  {
    if ((_displayBitmap >> i) & 1)
    {
      _display[i] = color1;
    }
    else
    {
      _display[i] = color0;
    }
  }
}


void Display::setDisplayColor0(const RgbLed &color)
{
  _color0 = color;
}


void Display::setDisplayColor1(const RgbLed &color)
{
  _color1 = color;
}


void Display::setDisplayFromRaw(const RgbLed *data)
{
  uint8_t i = 0;

  for (i = 0; i < cLedCount; i++)
  {
    _display[i] = data[i];
  }
  _displayBitmapMask = 0x000000;
}


void Display::setDisplayFromBitmap(const uint32_t bitmap)
{
  uint8_t i = 0;

  for (i = 0; i < cLedCount; i++)
  {
    if ((bitmap >> i) & 1)
    {
      _display[i] = _color1;
    }
    else
    {
      _display[i] = _color0;
    }
  }
  _displayBitmap = bitmap;
  _displayBitmapMask = 0xffffff;
}


void Display::setDisplayFromBytes(const uint8_t byte2, const uint8_t byte1, const uint8_t byte0)
{
  uint8_t  i = 0;
  uint32_t bitmap = ((uint32_t)byte2 << 16 ) | ((uint32_t)byte1 << 8) | (uint32_t)byte0;

  for (i = 0; i < cLedCount; i++)
  {
    if ((bitmap >> i) & 1)
    {
      _display[i] = _color1;
    }
    else
    {
      _display[i] = _color0;
    }
  }
  _displayBitmap = bitmap;
  _displayBitmapMask = 0xffffff;
}


void Display::setDisplayFromNibbles(const uint8_t byte5, const uint8_t byte4, const uint8_t byte3, const uint8_t byte2, const uint8_t byte1, const uint8_t byte0)
{
  uint8_t  i = 0;
  uint32_t bitmap = (((uint32_t)byte5 & 0x0f) << 20)
                  | (((uint32_t)byte4 & 0x0f) << 16)
                  | (((uint32_t)byte3 & 0x0f) << 12)
                  | (((uint32_t)byte2 & 0x0f) << 8)
                  | (((uint32_t)byte1 & 0x0f) << 4 )
                  | ((uint32_t)byte0 & 0x0f);

  for (i = 0; i < cLedCount; i++)
  {
    if ((bitmap >> i) & 1)
    {
      _display[i] = _color1;
    }
    else
    {
      _display[i] = _color0;
    }
  }
  _displayBitmap = bitmap;
  _displayBitmapMask = 0xffffff;
}


void Display::setLedFromState(const uint8_t ledNumber, const bool ledState)
{
  if (ledNumber < cLedCount)
  {
    if (ledState)
    {
      _display[ledNumber] = _color1;
    }
    else
    {
      _display[ledNumber] = _color0;
    }
  }
}


void Display::setLedFromRaw(const uint8_t ledNumber, const RgbLed &color)
{
  if (ledNumber < cLedCount)
  {
    _display[ledNumber] = color;
  }
}


bool Display::getLedState(const uint8_t ledNumber) const
{
  if (ledNumber < cLedCount)
  {
    if (_displayBitmap & (1 << ledNumber))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}


bool Display::getLedMask(const uint8_t ledNumber) const
{
  if (ledNumber < cLedCount)
  {
    if (_displayBitmapMask & (1 << ledNumber))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}


RgbLed Display::getLedRaw(const uint8_t ledNumber) const
{
  if (ledNumber < cLedCount)
  {
    RgbLed someRgbLed(_display[ledNumber]);

    return someRgbLed;
  }
  return 0;
}


};
