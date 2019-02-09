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


SetTimeDateView::SetTimeDateView()
  : _selectedItem(0),
    _workingDateTime(),
    _mode(Application::OperatingMode::OperatingModeSetClock),
    _settings(Settings())
{
}


void SetTimeDateView::enter()
{
  _mode = Application::getOperatingMode();

  _settings = Application::getSettings();

  switch (_mode)
  {
    case Application::OperatingMode::OperatingModeSetClock:
    case Application::OperatingMode::OperatingModeSetDate:
    _workingDateTime = Hardware::getDateTime();
    break;

    default:
    _workingDateTime = _settings.getTime(Application::getOperatingModeRelatedSetting(_mode));
    break;
  }

  DisplayManager::setStatusLedAutoRefreshing(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour));
}


bool SetTimeDateView::keyHandler(Keys::Key key)
{
  bool    tick   = true;
  int16_t year   = _workingDateTime.year();
  int8_t  month  = _workingDateTime.month(),
          day    = _workingDateTime.day(),
          hour   = _workingDateTime.hour(),
          minute = _workingDateTime.minute(),
          second = _workingDateTime.second();
  DateTime setDateTime = Hardware::getDateTime();

  if (key == Keys::Key::A)
  {
    if (_mode == Application::OperatingMode::OperatingModeSetDate)
    {
      setDateTime.setDate(year, month, day);
    }
    else
    {
      setDateTime.setTime(hour, minute, second);
    }

    switch (_mode)
    {
      case Application::OperatingMode::OperatingModeSetClock:
      case Application::OperatingMode::OperatingModeSetDate:
      Hardware::setDateTime(setDateTime);
      Hardware::setDstState(Application::isDst(setDateTime), false);
      break;

      default:
      _settings.setTime(Application::getOperatingModeRelatedSetting(_mode), setDateTime);
      Application::setSettings(_settings);
      break;
    }

    DisplayManager::doubleBlink();
  }

  if (key == Keys::Key::B)
  {
    if (--_selectedItem < 0)
    {
      _selectedItem = 2;
    }
  }

  if (key == Keys::Key::C)
  {
    if (++_selectedItem > 2)
    {
      _selectedItem = 0;
    }
  }

  if (key == Keys::Key::D)
  {
    // the setDate and setTime methods validate everything so we'll be lazy here
    if (_mode == Application::OperatingMode::OperatingModeSetDate)
    {
      switch (_selectedItem)
      {
        case HourYear:
        --year;
        break;

        case MinuteMonth:
        --month;
        break;

        default:
        --day;
      }
      _workingDateTime.setDate(year, month, day);
    }
    else
    {
      switch (_selectedItem)
      {
        case HourYear:
        --hour;
        break;

        case MinuteMonth:
        --minute;
        break;

        default:
        --second;
      }
      // this 'if' is just to prevent rolling over from 0 to 59
      if ((hour >= 0) && (minute >= 0) && (second >= 0))
      {
        _workingDateTime.setTime(hour, minute, second);
      }
    }
  }

  if (key == Keys::Key::U)
  {
    // the setDate and setTime methods validate everything so we'll be lazy here
    if (_mode == Application::OperatingMode::OperatingModeSetDate)
    {
      switch (_selectedItem)
      {
        case HourYear:
        ++year;
        break;

        case MinuteMonth:
        ++month;
        break;

        default:
        ++day;
      }
      _workingDateTime.setDate(year, month, day);
    }
    else
    {
      switch (_selectedItem)
      {
        case HourYear:
        ++hour;
        break;

        case MinuteMonth:
        ++minute;
        break;

        default:
        ++second;
      }
      _workingDateTime.setTime(hour, minute, second);
    }
  }

  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }

  return tick;
}


void SetTimeDateView::loop()
{
  RgbLed color0Highlight = _settings.getColor(Settings::Color::Color0, Settings::Slot::SlotSet),
         color1Highlight = _settings.getColor(Settings::Color::Color1, Settings::Slot::SlotSet),
         color0Lowlight = _settings.getColor(Settings::Color::Color0, Settings::Slot::SlotSet),
         color1Lowlight = _settings.getColor(Settings::Color::Color1, Settings::Slot::SlotSet),
         statusLed;
  bool displayBcd = _settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DisplayBCD),
       display12Hour = _settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour);
  uint8_t year  = _workingDateTime.yearShort(displayBcd),
          month = _workingDateTime.month(displayBcd),
          day   = _workingDateTime.day(displayBcd),
          highlightStart = _selectedItem;

  color0Lowlight.adjustIntensity(SetTimeDateView::cLowlightPercentage);
  color1Lowlight.adjustIntensity(SetTimeDateView::cLowlightPercentage);

  Display bcDisp(color0Lowlight, color1Lowlight, _workingDateTime.hour(displayBcd, display12Hour), _workingDateTime.minute(displayBcd), _workingDateTime.second(displayBcd));

  if (_mode == Application::OperatingMode::OperatingModeSetDate)
  {
    switch (_settings.getRawSetting(Settings::DateFormat))
    {
      case 2:
      bcDisp.setDisplayFromBytes(month, day, year);
      // determine what to highlight
      switch (_selectedItem)
      {
        case HourYear:
        highlightStart = 0;
        break;

        case MinuteMonth:
        highlightStart = 2;
        break;

        default:
        highlightStart = 1;
      }
      break;

      case 1:
      bcDisp.setDisplayFromBytes(day, month, year);
      // determine what to highlight
      switch (_selectedItem)
      {
        case HourYear:
        highlightStart = 0;
        break;

        case MinuteMonth:
        highlightStart = 1;
        break;

        default:
        highlightStart = 2;
      }
      break;

      default:
      bcDisp.setDisplayFromBytes(year, month, day);
      // no need to adjust highlightStart as _selectedItem directly corresponds to this arrangement
    }
  }
  else if (display12Hour == true)
  {
    if (_workingDateTime.isPM() == true)
    {
      statusLed = color1Highlight;
    }
    else
    {
      statusLed = color0Highlight;
    }
    // highlight/lowlight as appropriate
    if (_selectedItem != HourYear)
    {
      statusLed.adjustIntensity(SetTimeDateView::cLowlightPercentage);
    }

  }

  // create the highlight on the appropriate block of pixels
  for (uint8_t i = highlightStart * 8; i < (highlightStart + 1) * 8; i++)
  {
    if (bcDisp.getPixelState(i) == true)
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
