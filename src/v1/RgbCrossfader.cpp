//
// kbx81's binary clock RgbCrossfader Library
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
#include "RgbCrossfader.h"
#include "RgbLed.h"

/// @mainpage
///
/// @section intro_sec Introduction
///
/// This library contains a class to manage crossfading of LEDs on the Binary
/// Clock's RGB LED display.
///
/// @section requirements_sec Requirements
///
/// This library is written in a manner so as to be compatible on a range of
/// CPUs/MCUs. It has been tested on Arduino and STM32F0 platforms. It requires
/// a modern C++ compiler (C++11).
///
/// @section classes_sec Classes
///
/// There is only the RgbCrossfader::RgbCrossfader class. Read the documentation
/// of this class for all details.
///

namespace kbxBinaryClock {


RgbCrossfader::RgbCrossfader()
{
}


bool RgbCrossfader::fadeIsActive()
{
  return (_active.getDuration() < _target.getDuration());
}


RgbLed RgbCrossfader::getActive() const
{
  return _active;
}


RgbLed RgbCrossfader::getTargetWithDuration() const
{
  return _target;
}


void RgbCrossfader::resetFade()
{
  // reset the crossfade timer/counter
  _active.setDuration(0);
  // where we are is where the fade starts now
  _start = _active;
}


void RgbCrossfader::startNewFade(const RgbLed &newTarget)
{
  _target = newTarget;
  resetFade();
}


void RgbCrossfader::startNewFadeIfDifferent(const RgbLed &newTarget)
{
  if (newTarget != _target)
  {
    _target = newTarget;
    resetFade();
  }
}


void RgbCrossfader::tick()
{
  // the _active RgbLed object's Duration is the current number of ticks into the fade
  uint32_t currentTick  = _active.getDuration(),
           totalTicks   = _target.getDuration(),
           percentTicks = RgbLed::cLed100Percent;

  if (currentTick < totalTicks)
  {
    if (totalTicks != 0)  // do not divide by zero omg
    {
      percentTicks = (RgbLed::cLed100Percent * currentTick) / totalTicks;
    }
    // set _active to the merged values of _start and _target
    _active.setFromMergedRgbLeds(percentTicks, _start, _target);
    _active.setDuration(currentTick + 1);
  }
  else if (currentTick >= totalTicks)
  {
    // ensure _active has reached the _target values
    _active = _target;
    // this kicks us into a "do nothing" state
    // _active.setDuration(currentTick + 1);
  }
}


void RgbCrossfader::updateFadeDuration(const uint32_t newDuration)
{
  _target.setDuration(newDuration);
}


void RgbCrossfader::updateFadeDuration(const RgbLed &newDuration)
{
  _target.setDuration(newDuration.getDuration());
}


void RgbCrossfader::updateFadeTarget(const RgbLed &newTarget, const bool startNewFade)
{
  _target.setRGB(newTarget.getRed(), newTarget.getGreen(), newTarget.getBlue());

  if (startNewFade == true)
  {
    resetFade();
  }
}


void RgbCrossfader::updateFadeTargetAndDuration(const RgbLed &newTarget, const bool startNewFade)
{
  _target = newTarget;

  if (startNewFade == true)
  {
    resetFade();
  }
}


};
