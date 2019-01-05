//
// kbx81's binary clock TimeDateTempView class
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

#include "Application.h"
#include "DateTime.h"
#include "Settings.h"
#include "View.h"


namespace kbxBinaryClock {

class TimeDateTempView : public View {

enum FixedDisplayItem : uint8_t
{
  Time = ViewMode::ViewMode0,
  TimeSeconds = ViewMode::ViewMode1,
  Date = ViewMode::ViewMode2,
  Temperature = ViewMode::ViewMode3
};

// The view which displays either the date, time, temperature, or a rotation
//
public: // Implement the TimeDateTempView class
  TimeDateTempView();
  virtual void enter() override;
  virtual bool keyHandler(Keys::Key key) override;
  virtual void loop() override;

private:
  // number of seconds in a day
  //
  static const uint32_t cSecondsInADay;

  // amount by which we increment/decrement the intensity when manually adjusting
  //
  static const uint8_t cIntensityAdjustmentIncrement;

  // tracks the last time the display was switched
  //
  uint32_t _lastSwitchTime;

  // temperature
  //
  int32_t _currentTemperature;

  // date & time
  //
  DateTime _currentTime;

  // the main application's mode
  //
  Application::OperatingMode _mode;

};

}
