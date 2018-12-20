//
// kbx81's binary clock set time/date view class
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
#include "Application.h"
#include "DateTime.h"
#include "DisplayManager.h"
#include "Hardware.h"
#include "Settings.h"
#include "SetTimeDateView.h"


namespace kbxBinaryClock {

// Percentage of configured LED color intensities used for lowlight
//  (Percentages are times 100 -- e.g.: 2500 = 25.00%)
const uint16_t SetTimeDateView::cLowlightPercentage = 2500;


void SetTimeDateView::enter()
{
  DateTime setDateTime;

  _mode = Application::getOperatingMode();

  _settings = Application::getSettings();

  switch (_mode)
  {
    case Application::OperatingMode::OperatingModeSetClock:
    case Application::OperatingMode::OperatingModeSetDate:
    setDateTime = Hardware::getDateTime();
    break;

    default:
    // Subtract the first in the OperatingModeSlotxTime series from the
    //   current mode to get the slot we're modifying
    setDateTime = _settings.getTime(_mode - Application::OperatingMode::OperatingModeSlot1Time);
    break;
  }

  if (_mode == Application::OperatingMode::OperatingModeSetDate)
  {
    _setValues[2] = setDateTime.yearShort(false);
    _setValues[1] = setDateTime.month(false);
    _setValues[0] = setDateTime.day(false);

    _maxValues[2] = 99;
    _maxValues[1] = 12;
    _maxValues[0] = setDateTime.daysThisMonth();
  }
  else
  {
    _setValues[2] = setDateTime.hour(false, false);
    _setValues[1] = setDateTime.minute(false);
    _setValues[0] = setDateTime.second(false);

    _maxValues[2] = 23;
    _maxValues[1] = 59;
    _maxValues[0] = 59;

    DisplayManager::setStatusLedAutoRefreshing(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour));
  }
}


void SetTimeDateView::keyHandler(Keys::Key key)
{
  DateTime setDateTime = Hardware::getDateTime();

  if (key == Keys::Key::A)
  {
    if (_mode == Application::OperatingMode::OperatingModeSetDate)
    {
      setDateTime.setDate(2000 + _setValues[2], _setValues[1], _setValues[0]);
    }
    else
    {
      setDateTime.setTime(_setValues[2], _setValues[1], _setValues[0]);
    }

    switch (_mode)
    {
      case Application::OperatingMode::OperatingModeSetClock:
      case Application::OperatingMode::OperatingModeSetDate:
      Hardware::setDateTime(setDateTime);
      Hardware::setDstState(Application::isDst(setDateTime), false);
      break;

      default:
      // Subtract the first in the OperatingModeSlotxTime series from the
      //   current mode to get the slot we're modifying
      _settings.setTime(_mode - Application::OperatingMode::OperatingModeSlot1Time, setDateTime);
      Application::setSettings(_settings);
      break;
    }

    DisplayManager::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
  }

  if (key == Keys::Key::C)
  {
    if (++_selectedByte > 2)
    {
      _selectedByte = 0;
    }
  }

  if (key == Keys::Key::D)
  {
    if (_mode == Application::OperatingMode::OperatingModeSetDate)
    {
      // any given value can't be less than one
      if (--_setValues[_selectedByte] < 1)
      {
        _setValues[_selectedByte] = 1;
      }

      if (_selectedByte > 0)   // month or year is selected...
      {
        setDateTime.setDate(2000 + _setValues[2], _setValues[1], _setValues[0]);
        _maxValues[0] = setDateTime.daysThisMonth();   // update max days in the month
        if (_setValues[0] >= _maxValues[0])   // if selected day > max days...
        {
          _setValues[0] = _maxValues[0];  // ...fix it
        }
      }
    }
    else
    {
      if (--_setValues[_selectedByte] > _maxValues[_selectedByte])
      {
        _setValues[_selectedByte] = 0;
      }
    }
  }

  if (key == Keys::Key::U)
  {
    if (++_setValues[_selectedByte] > _maxValues[_selectedByte])
    {
      _setValues[_selectedByte] = _maxValues[_selectedByte];
    }

    if (_mode == Application::OperatingMode::OperatingModeSetDate && _selectedByte > 0)   // month or year is selected
    {
      setDateTime.setDate(2000 + _setValues[2], _setValues[1], _setValues[0]);
      _maxValues[0] = setDateTime.daysThisMonth();   // update max days in the month
      if (_setValues[0] > _maxValues[0])   // if selected day > max days...
      {
        _setValues[0] = _maxValues[0];  // ...fix it
      }
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void SetTimeDateView::loop()
{
  RgbLed color0Highlight = _settings.getColor0(Settings::Slot::SlotSet),
         color1Highlight = _settings.getColor1(Settings::Slot::SlotSet),
         color0Lowlight = _settings.getColor0(Settings::Slot::SlotSet),
         color1Lowlight = _settings.getColor1(Settings::Slot::SlotSet),
         statusLed;
  uint8_t  i, adjustedHour = _setValues[2],
           workingByte = _setValues[_selectedByte];
  uint32_t displayBitMask;

  color0Lowlight.adjustIntensity(SetTimeDateView::cLowlightPercentage);
  color1Lowlight.adjustIntensity(SetTimeDateView::cLowlightPercentage);

  // display AM/PM on the status LED if we're in 12-hour mode and adjust
  //  the displayed hour to match what's expected
  if ((_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour) == true) &&
      (_mode != Application::OperatingMode::OperatingModeSetDate))
  {
    if (adjustedHour >= 12)
    {
      adjustedHour = adjustedHour - 12;
      statusLed = _settings.getColor1(Settings::Slot::SlotSet);
    }
    else
    {
      statusLed = _settings.getColor0(Settings::Slot::SlotSet);
    }

    if (adjustedHour == 0)
    {
      adjustedHour = 12;
    }

    if (_selectedByte == 2)
    {
      workingByte = adjustedHour;
    }
  }

  if (_selectedByte != 2)
  {
    statusLed.adjustIntensity(SetTimeDateView::cLowlightPercentage);
  }

  // display in BCD if settings say so
  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true)
  {
    displayBitMask = (Hardware::uint32ToBcd(adjustedHour) << 16) |
                      (Hardware::uint32ToBcd(_setValues[1]) << 8) |
                       Hardware::uint32ToBcd(_setValues[0]);
  }
  else
  {
    displayBitMask = (adjustedHour << 16) | (_setValues[1] << 8) | _setValues[0];
  }

  // now we can create a new display object with the right colors and bitmask
  Display bcDisp(color0Lowlight, color1Lowlight, displayBitMask);

  // get the bitmap for the selected byte into displayBitMask
  if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD) == true)
  {
    displayBitMask = Hardware::uint32ToBcd(workingByte);
  }
  else
  {
    displayBitMask = workingByte;
  }
  // create the highlight
  for (i = _selectedByte * 8; i < (_selectedByte + 1) * 8; i++)
  {
    if ((displayBitMask >> (i - (_selectedByte * 8))) & 1)
    {
      bcDisp.setPixelFromRaw(i, color1Highlight);
    }
    else
    {
      bcDisp.setPixelFromRaw(i, color0Highlight);
    }
  }

  DisplayManager::writeDisplay(bcDisp, statusLed);
}


}
