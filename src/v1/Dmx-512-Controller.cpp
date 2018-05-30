//
// kbx81's binary clock DMX-512 controller class
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
#include "Application.h"
#include "Dmx-512-Controller.h"
#include "Dmx-512-Packet.h"
#include "Dmx-512-Rx.h"
#include "Hardware.h"
#include "Settings.h"
#include "View.h"


namespace kbxBinaryClock {

namespace Dmx512Controller {


// Duration of strobe pulses (here, in milliseconds)
//  Time unit depends on frequency of calls to strobeTimer()
static const uint16_t cStrobeDuration = 1;

// The application's current settings
//
static Settings *_pSettings;

// Master intensity level
//
volatile static uint8_t _masterIntensityLevel;

// Counter for strobe delays
//
volatile static uint16_t _strobeCounter;

// Delay between strobe pulses (here, in milliseconds)
//  Time unit depends on frequency of calls to strobeTimer()
volatile static uint16_t _strobeDelay;


void initialize()
{
  _pSettings = Application::getSettingsPtr();
}


void controller()
{
  Dmx512Packet* currentPacket = Dmx512Rx::getLastPacket();
  uint16_t pitch, rate, address = _pSettings->getRawSetting(Settings::Setting::DmxAddress);
  uint8_t level;
  RgbLed dmxLed[2];
  const Application::OperatingMode opMode[] = {Application::OperatingMode::OperatingModeDmx512Display,
                                               Application::OperatingMode::OperatingModeFixedDisplay,
                                               Application::OperatingMode::OperatingModeFixedDisplay,
                                               Application::OperatingMode::OperatingModeFixedDisplay,
                                               Application::OperatingMode::OperatingModeTimerCounter,
                                               Application::OperatingMode::OperatingModeTimerCounter,
                                               Application::OperatingMode::OperatingModeTimerCounter,
                                               Application::OperatingMode::OperatingModeTimerCounter};
  const ViewMode viewMode[] = {ViewMode::ViewMode0,
                               ViewMode::ViewMode0,
                               ViewMode::ViewMode2,
                               ViewMode::ViewMode3,
                               ViewMode::ViewMode0,
                               ViewMode::ViewMode1,
                               ViewMode::ViewMode2,
                               ViewMode::ViewMode3};

  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    if (currentPacket->startCode() == 0)
    {
      if (_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DmxExtended) == true)
      {
        level = currentPacket->channel(address++) / 32;
        Application::setOperatingMode(opMode[level]);
        Application::setViewMode(viewMode[level]);

        level = currentPacket->channel(address++) >> 5;
        Hardware::setVolume(level);

        pitch = currentPacket->channel(address++);
        Hardware::tone(pitch * 16, 60);

        _strobeDelay = currentPacket->channel(address++) * 4;
        if (_strobeDelay > 0)
        {
          _strobeDelay += 20;
        }

        rate = currentPacket->channel(address++) * 4;
        dmxLed[0].setRate(rate);
        dmxLed[1].setRate(rate);

        // Set display blanking/driver current level
        _masterIntensityLevel = currentPacket->channel(address++) / 52;

        dmxLed[0].setRed(currentPacket->channel(address++) << 4);
        dmxLed[0].setGreen(currentPacket->channel(address++) << 4);
        dmxLed[0].setBlue(currentPacket->channel(address++) << 4);

        dmxLed[1].setRed(currentPacket->channel(address++) << 4);
        dmxLed[1].setGreen(currentPacket->channel(address++) << 4);
        dmxLed[1].setBlue(currentPacket->channel(address++) << 4);

        _pSettings->setColors(Settings::Slot::SlotDmx, dmxLed[0], dmxLed[1]);

        if (_masterIntensityLevel > 0)
        {
          Hardware::currentDrive(_masterIntensityLevel - 1);
        }
      }
      else
      {
        // Set display blanking/driver current level
        _masterIntensityLevel = currentPacket->channel(address) / 52;
      }
    }
  }
}


void strobeTimer()
{
  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    if (_strobeDelay > 0)
    {
      _strobeCounter++;
    }

    if (((_masterIntensityLevel > 0) && (_strobeDelay == 0)) ||
        ((_masterIntensityLevel > 0) && (_strobeDelay > 0) && (_strobeCounter > _strobeDelay)))
    {
      Hardware::displayBlank(false);
      if (_strobeCounter > _strobeDelay + cStrobeDuration)
      {
        _strobeCounter = 0;
      }
    }
    else
    {
      Hardware::displayBlank(true);
    }
  }
}


}

}
