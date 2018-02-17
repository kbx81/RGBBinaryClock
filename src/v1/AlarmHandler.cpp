//
// kbx81's binary clock alarm handler
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
#include <cstdint>

#include "AlarmHandler.h"
#include "Application.h"
#include "DateTime.h"
#include "Hardware.h"
#include "Keys.h"
#include "Settings.h"


namespace kbxBinaryClock {

namespace AlarmHandler {


// How frequently we will beep
//
const uint16_t cBeepShortInterval = 75;

// How frequently we will beep
//
const uint16_t cBeepLongInterval = 700;

// Number of beeps between pauses for alarms
//
const uint8_t cAlarmBeeps = 4;

// Number of beeps for hourly chime
//
const uint8_t cHourlyBeeps = 2;

// Timer for beeping
//
uint16_t _beepTimer = 0;

// Counters for beeps
//
uint16_t _beepCounter = 0;

// Indicates an alarm is active
//
uint8_t _activeAlarms = 0;

// Times at which each alarm was last acknowledged
//
DateTime _ackTime[8];

// The application's current settings
//
Settings _settings;

// Indicates hourly alarm should sound
//  Initial state of 'true' produces a nice start-up beep
bool _hourlyAlarmActive = true;


void keyHandler(Keys::Key key)
{
  DateTime current = Hardware::getDateTime();

  if (key == Keys::Key::A)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      if (_activeAlarms & (1 << i))
      {
        _ackTime[i] = current;
      }
    }
    _activeAlarms = 0;
    _beepCounter = 0;
    _beepTimer = 0;
  }
}


// Called from main application loop
//
void loop()
{
  DateTime slot, current = Hardware::getDateTime();

  if (Hardware::rtcIsSet())
  {
    if (_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::HourlyChime) &&
        !current.second(false) && !current.minute(false))
    {
      _hourlyAlarmActive = true;
    }

    for (uint8_t i = static_cast<uint8_t>(Settings::Slot::Slot1); i <= static_cast<uint8_t>(Settings::Slot::Slot8); i++)
    {
      // first check if the alarm is enabled
      if (_settings.getSetting(Settings::Setting::BeepStates, i)) // || _settings.getSetting(Settings::Setting::BlinkStates, i))
      {
        // get the current slot's time
        slot = _settings.getTime(i);

        // if the current time is after the alarm slot's time...
        if (current.secondsSinceMidnight(false) >= slot.secondsSinceMidnight(false))
        {
          // set the alarm slot's date to today
          slot.setDate(current.year(false), current.month(false), current.day(false));

          // if the time it was last acknowledged is before the alarm slot's time...
          if (_ackTime[i] < slot)
          {
            // ...we have an active alarm
            _activeAlarms |= (1 << i);
          }
        }
      }
    }
  }
}


// Provides a way to determine if an alarm is active
//
bool isAlarmActive()
{
  return _activeAlarms;
}


// Allows the main application to update our settings as needed
//
void setSettings(Settings settings)
{
  _settings = settings;
}


// Refreshes the state of the alarm/beeper, permitting multiple beeps
//  per alarm/alarm type
void refreshAlarm()
{
  if (_activeAlarms)
  {
    // cancel this if other alarm is active
    _hourlyAlarmActive = false;

    if (_beepCounter >= cAlarmBeeps)
    {
      if (_beepTimer++ >= cBeepLongInterval)
      {
        _beepCounter = 0;
        _beepTimer = 0;
      }
    }
    else
    {
      if (_beepTimer++ >= cBeepShortInterval)
      {
        _beepTimer = 0;

        if (Hardware::beepState())
        {
          Hardware::beep(false);
          _beepCounter++;
        }
        else
        {
          Hardware::beep(true);
        }
      }
    }
  }
  else if (_hourlyAlarmActive)
  {
    if (_beepCounter >= cHourlyBeeps)
    {
      if (_beepTimer++ >= cBeepLongInterval)
      {
        _beepCounter = 0;
        _beepTimer = 0;
        _hourlyAlarmActive = false;
      }
    }
    else
    {
      if (_beepTimer++ >= cBeepShortInterval)
      {
        _beepTimer = 0;

        if (Hardware::beepState())
        {
          Hardware::beep(false);
          _beepCounter++;
        }
        else
        {
          Hardware::beep(true);
        }
      }
    }
  }
  else
  {
    Hardware::beep(false);
  }
}


}

}
