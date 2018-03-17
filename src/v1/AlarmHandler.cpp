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

#include <libopencm3/stm32/rtc.h>
#include "AlarmHandler.h"
#include "Application.h"
#include "DateTime.h"
#include "Hardware.h"
#include "Keys.h"
#include "Settings.h"


namespace kbxBinaryClock {

namespace AlarmHandler {


// Alarm tone durations and frequencies
//
const uint16_t cHourlyAlarmToneDuration = 75;

const uint16_t cAlarmToneDurations[]   = {75, 75, 75, 75, 75, 75, 75, 700};
const uint16_t cAlarmToneFrequencies[] = {1, 0, 1, 0, 1, 0, 1, 0};

// Number of alarms available
//
const uint8_t _alarmCount = 8;

// Number of beeps for hourly chime * 2
//
const uint8_t cHourlyBeeps = 4;

// Counter for alarm beeps
//
uint8_t _beepCounter = 0;

// Indicates which alarm(s) is/are active
//
uint8_t _activeAlarms = 0;

// Indicates hourly alarm should sound
//  Initial state of 'true' produces a nice start-up beep
bool _hourlyAlarmActive = true;

// Indicates the last hour during which the hourly alarm sounded
//  Initial value is not a valid hour so it will always fire after startup
uint8_t _lastHourlyBeepHour = 25;

// Times at which each alarm was last acknowledged
//
DateTime _ackTime[_alarmCount];

// The application's current settings
//
Settings _settings;


void initialize()
{
  for (uint8_t i = 0; i < _alarmCount; i++)
  {
    _ackTime[i] = Hardware::getDateTime();
  }
}


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
    // restore the display in case the alarm changed anything
    Hardware::autoAdjustIntensities(_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::AutoAdjustIntensity));
    Hardware::displayBlank(false);
  }
}


// Called from main application loop
//
void loop()
{
  uint8_t i;
  DateTime slot, current = Hardware::getDateTime();

  // first, check if any alarm boundaries have been crossed, triggering an alarm
  if (Hardware::rtcIsSet())
  {
    // if the minutes and seconds are both zero, it's the top of the hour
    if ((_settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::HourlyChime) == true) &&
        (current.second(false) == 0) && (current.minute(false) == 0))
    {
      _hourlyAlarmActive = true;
    }

    for (i = static_cast<uint8_t>(Settings::Slot::Slot1); i <= static_cast<uint8_t>(Settings::Slot::Slot8); i++)
    {
      // first check if the alarm is enabled
      if ((_settings.getSetting(Settings::Setting::BeepStates, i) == true) || (_settings.getSetting(Settings::Setting::BlinkStates, i) == true))
      {
        // get the current slot's time
        slot = _settings.getTime(i);

        // if the current time is after the alarm slot's time...
        if (current.secondsSinceMidnight(false) >= slot.secondsSinceMidnight(false))
        {
          // set the alarm slot's date to today so we can compare them
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

    // next, make the beeper beep if it's time to do so
    if (_activeAlarms > 0)
    {
      for (i = static_cast<uint8_t>(Settings::Slot::Slot1); i <= static_cast<uint8_t>(Settings::Slot::Slot8); i++)
      {
        // first check if the alarm is enabled
        if (_settings.getSetting(Settings::Setting::BeepStates, i) == true)
        {
          // cancel this since some other, more important alarm is active
          _hourlyAlarmActive = false;

          if (_beepCounter >= sizeof(cAlarmToneDurations) / 2) // words / 2 = bytes
          {
            _beepCounter = 0;
          }

          if (Hardware::tone(cAlarmToneFrequencies[_beepCounter], cAlarmToneDurations[_beepCounter]) == true)
          {
            _beepCounter++;
          }
        }

        // first check if the alarm is enabled
        if (_settings.getSetting(Settings::Setting::BlinkStates, i) == true)
        {
          // make the display bright!
          Hardware::autoAdjustIntensities(false);
          // blink based on seconds, or...
          // Hardware::displayBlank(current.second(false) & 1);
          // blink based on sub-seconds register for faster blinking
          Hardware::displayBlank(RTC_SSR & (1 << 6));
        }
      }
    }
    else if (_hourlyAlarmActive == true)
    {
      uint8_t hour = current.hour(false, _settings.getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::Display12Hour));

      if (hour == _lastHourlyBeepHour)
      {
        // ignore the request if we already beeped this hour
        _hourlyAlarmActive = false;
      }
      else
      {
        if (_beepCounter >= cHourlyBeeps)
        {
          _beepCounter = 0;
          _lastHourlyBeepHour = hour;
          _hourlyAlarmActive = false;
        }
        else if (Hardware::tone(_beepCounter & 1, cHourlyAlarmToneDuration) == true)
        {
          _beepCounter++;
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


}

}
