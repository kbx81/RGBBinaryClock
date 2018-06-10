//
// kbx81's binary clock DMX-512 View class
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
#include "Display.h"
#include "Dmx-512-Packet.h"
#include "Dmx-512-Rx.h"
#include "Dmx-512-View.h"
#include "Settings.h"
#include "View.h"


namespace kbxBinaryClock {


void Dmx512View::enter()
{
  _mode = Application::getOperatingMode();

  _pSettings = Application::getSettingsPtr();

  Hardware::displayBlank(true);
  // make the display bright so we can see the colors we're working with
  Hardware::autoAdjustIntensities(false);

  Hardware::autoRefreshStatusLed(true);
}


void Dmx512View::keyHandler(Keys::Key key)
{
  if (key == Keys::Key::E)
  {
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


void Dmx512View::loop()
{
  Dmx512Packet* currentPacket = Dmx512Rx::getLastPacket();
  Display dmxDisplay;
  RgbLed currentLed;
  uint16_t rate, address = _pSettings->getRawSetting(Settings::Setting::DmxAddress);

  if (Application::getExternalControlState() == Application::ExternalControl::Dmx512ExtControlEnum)
  {
    if (currentPacket->startCode() == 0)
    {
      if (_pSettings->getSetting(Settings::Setting::SystemOptions, Settings::SystemOptionsBits::DmxExtended) == true)
      {
        address += 4;

        rate = (currentPacket->channel(address++) * cChannelMultiplier) + cChannelMultiplier;
        currentLed.setRate(rate);
      }
      // Skip over the display blanking/driver current level parameter
      address++;

      for (uint8_t i = 0; i <= Display::cLedCount; i++)
      {
        currentLed.setRed(currentPacket->channel((i * 3) + address) << cChannelMultiplier);
        currentLed.setGreen(currentPacket->channel((i * 3) + address + 1) << cChannelMultiplier);
        currentLed.setBlue(currentPacket->channel((i * 3) + address + 2) << cChannelMultiplier);
        if (i < Display::cLedCount)
        {
          dmxDisplay.setLedFromRaw(i, currentLed);
        }
        else
        {
          Hardware::setStatusLed(currentLed);
        }
      }
      Hardware::writeDisplay(dmxDisplay);
    }
  }
  else
  {
    // Kick us back to the main menu if the signal was lost
    Application::setOperatingMode(Application::OperatingMode::OperatingModeMainMenu);
  }
}


}
