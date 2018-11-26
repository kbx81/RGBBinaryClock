//
// kbx81's binary clock Hardware class
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

#include <cstddef>
#include <cstdint>
#include "DateTime.h"
#include "Display.h"
#include "RgbLed.h"


namespace kbxBinaryClock {

namespace Hardware {

  // SPI1 peripherals
  //
  enum SpiPeripheral : uint8_t {
    NotInitialized,
    LedDriversGs,
    LedDriversOther,
    Rtc,
    TempSensor
  };


  // Initialize the hardware
  //
  void     initialize();

  // Refreshes hardware external to the MCU
  //
  void     refresh();

  // Generates a tick sound if no tone is active
  //
  bool     tick();

  // Generates a tone of frequency for duration milliseconds
  //  Returns true if tone was activated
  bool     tone(const uint16_t frequency, const uint16_t duration);

  // Returns true if an external alarm input pin is active (they are active low)
  //
  bool     alarmInput(const uint8_t alarmInputNumber);

  // Check a button
  //
  bool     button(const uint8_t button);

  // Returns a bitmap indicating the state of all six buttons
  //
  uint8_t  buttons();

  // Returns a bitmap indicating buttons that were pressed since the last check
  //
  uint8_t  buttonsPressed();

  // Returns a bitmap indicating buttons that were released since the last check
  //
  uint8_t  buttonsReleased();

  // Refreshes the button states from the TSC
  //
  void     buttonsRefresh();

  // Set the current drive level for the drivers
  //
  void     setLedCurrentDrive(const uint8_t drive);

  // Enable or disable the display blanking output pin
  //
  void     setDisplayBlankPin(const bool pinState);

  // Returns the state of daylight savings time in hardware
  //
  bool     dstState();

  // Get the current date/time
  //
  DateTime getDateTime();

  // Set the date/time
  //
  void     setDateTime(const DateTime &dateTime);

  // Returns the state of the RTC (true if year != 0)
  //
  bool     rtcIsSet();

  // Returns the temperature based on hardware
  //  (in fahrenheit if fahrenheit == true; in BCD if bcd == true)
  int32_t  temperature(const bool fahrenheit, const bool bcd);

  // Returns the level of light seen by the phototransistor (0 = min, 4095 = max)
  //
  uint16_t lightLevel();

  // Adjusts the clock for daylight savings
  //
  void     setDstState(const bool enableDst, const bool adjustRtcHardware);

  // Sets the value used as a threshold for flicker reduction at low intensities
  //
  void     setFlickerReduction(const uint16_t value);

  // Sets the calibration value used for the temperature calculation
  //
  void     setTemperatureCalibration(const int8_t value);

  // Sets the volume of tones emitted from the buzzer
  //  Valid range is 0 (muted/minimum) to 7 (maximum/default)
  void     setVolume(const uint8_t volumeLevel);

  // Erases the FLASH area used for app data
  //
  uint32_t eraseFlash(uint32_t startAddress);

  // Reads the FLASH area used for app data
  //
  void     readFlash(uint32_t startAddress, uint16_t numElements, uint8_t *outputData);

  // Writes the FLASH area used for app data
  //  Returns 0xff if startAddress is out of range, flash_get_status_flags() if
  //  erase request failed, 0x40 if erase verification failed,
  //  (0x80 | flash_get_status_flags()) if programming failure
  uint32_t writeFlash(uint32_t startAddress, uint8_t *inputData, uint16_t numElements);

  // Reads data from the serial port with DMA
  //  Returns false if failure (the USART was busy)
  bool     readSerial(const uint32_t usart, const uint32_t length, char* data);

  // Writes data to the serial port with DMA
  //  Returns false if failure (the USART was busy)
  bool     writeSerial(const uint32_t usart, const uint32_t length, const char* data);

  // Transfers data in/out through the SPI via DMA
  //  Returns false if failure (the SPI was busy)
  bool     spiTransfer(const SpiPeripheral peripheral, uint8_t *bufferIn, uint8_t *bufferOut, const uint16_t length, const bool use16BitXfers);

  // Permits checking the status of the SPI; returns true if busy
  //
  bool     spiIsBusy();

  // Sets the given status LED to the given intensity/RgbLed
  //
  void     setBlueLed(const uint32_t intensity);
  void     setGreenLed(const uint32_t intensity);
  void     setRedLed(const uint32_t intensity);
  void     setStatusLed(const RgbLed led);
  void     blinkStatusLed(const RgbLed led1, const RgbLed led2, uint32_t numberOfBlinks, const uint32_t delayLength);

  // Creates a delay of length based on systick (configured for milliseconds)
  //
  void     delay(const uint32_t length);

  // Converts to/from int/BCD
  //
  uint32_t bcdToUint32(uint32_t bcdValue);
  uint32_t uint32ToBcd(uint32_t uint32Value); // beware of values > 99999999


  // Interrupt Service Routines
  //
  void     dmaIsr();
  void     systickIsr();
  void     tim2Isr();
  void     tim15Isr();
  void     tim16Isr();
  void     tscIsr();
  void     usart1Isr();
  void     usart2Isr();

}

}
