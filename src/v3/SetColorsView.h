//
// kbx81's binary clock set colors view class
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
#include "Settings.h"
#include "View.h"


namespace kbxBinaryClock {

class SetColorsView : public View {

  enum SCVDisplayItem : uint8_t
  {
    ColorMixer = ViewMode::ViewMode0,
    DeltaValue = ViewMode::ViewMode1
  };

// The view which displays the UI for setting colors
//
public: // Implement the SetColorsView class
// @brief Stored color values for time slots
//
  enum Color : uint8_t
  {
    Red0 = 0,
    Green0 = 1,
    Blue0 = 2,
    Red1 = 3,
    Green1 = 4,
    Blue1 = 5
  };

  virtual void enter() override;
  virtual void keyHandler(Keys::Key key) override;
  virtual void loop() override;

private:
  // maximum amount we'll increase/decrease the color values by
  //
  static const uint16_t cMaxDelta;

  // how much we'll shift (left) by
  //
  static const uint8_t cDeltaShiftAmt;

  // values the user will set
  //
  uint16_t _setValues[6];

  // the selected parameter
  //
  uint8_t _selectedParam;

  // value by which the up/down key will move the selected parameter
  //
  uint16_t _delta;

  // the main application's mode
  //
  Application::OperatingMode _mode;

  // Settings to be used by the view
  //
  Settings _settings;

};

}
