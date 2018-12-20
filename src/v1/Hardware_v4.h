//
// kbx81's binary clock Hardware class pin definitions
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
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>


namespace kbxBinaryClock {

namespace Hardware {

  // USART Tx and Rx ports & pins
  //
  // static const auto cUsart1Port = GPIOA;
  static const auto cUsart1RxPort = GPIOA;
  static const auto cUsart1RxPin  = GPIO10;
  static const auto cUsart1TxPort = GPIOB;
  static const auto cUsart1TxPin  = GPIO6;

  static const auto cUsart2Port   = GPIOA;
  // static const auto cUsart2RxPort = GPIOA;
  static const auto cUsart2RxPin  = GPIO3;
  // static const auto cUsart2TxPort = GPIOA;
  static const auto cUsart2TxPin  = GPIO2;
  // static const auto cUsart2DePort = GPIOA;
  static const auto cUsart2DePin  = GPIO1;

  // external alarm input port(s) & pins
  //
  static const auto     cAlarmInputPort     = GPIOC;
  static const uint8_t  cAlarmInputPinCount = 4;
  static const uint16_t cAlarmInputPins[cAlarmInputPinCount] = { GPIO1, GPIO4, GPIO9, GPIO13 };

  // buzzer I/O port & pin
  //
  static const auto cBuzzerPort = GPIOA;
  static const auto cBuzzerPin  = GPIO8;

  // status LED I/O port(s) & pins
  //
  static const auto cLedPort    = GPIOC;
  static const auto cLedAltPort = GPIOC;
  static const auto cLed0Pin    = GPIO6;
  static const auto cLed1Pin    = GPIO7;
  static const auto cLed2Pin    = GPIO8;

  static const auto cLedPortAF  = GPIO_AF0;

  static const auto cLedPwmTimer = TIM3;
  static const auto cLed0PwmOc   = TIM_OC1;
  static const auto cLed1PwmOc   = TIM_OC2;
  static const auto cLed2PwmOc   = TIM_OC3;

  // phototransistor I/O port, pin, and ADC channel
  //
  static const auto cPhototransistorPort = GPIOC;
  static const auto cPhototransistorPin  = GPIO0;
  static const uint8_t cPhototransistorChannel = 10;

  // I2C ports & pins
  //
  static const auto cI2c1Port   = GPIOB;
  static const auto cI2c1SlcPin = GPIO8;
  static const auto cI2c1SdaPin = GPIO9;

  // SPI ports & pins
  //
  static const auto cSpi1Port           = GPIOA;
  static const auto cSpi1AltPort        = GPIOB;
  static const auto cSpi1SckPin         = GPIO5;
  static const auto cSpi1MisoPin        = GPIO6;
  static const auto cSpi1MosiPin        = GPIO7;
  static const auto cSpi1MisoDisplayPin = GPIO4;

  // SPI NSS I/O port & pins
  //
  static const auto cNssPort           = GPIOC;
  static const auto cNssDisplayPin     = GPIO10;
  static const auto cNssRtcPin         = GPIO11;
  static const auto cNssTemperaturePin = GPIO12;

  // driver blank I/O port & pin
  //   on the TLC5951 this is named XBLNK
  static const auto cBlankDisplayPort = GPIOB;
  static const auto cBlankDisplayPin  = GPIO3;

  // current drive I/O port & pins
  //
  static const auto cCurrentDrivePort    = GPIOB;
  static const auto cCurrentDriveLowPin  = GPIO6;
  static const auto cCurrentDriveHighPin = GPIO7;

  // driver GSCK I/O port & pin
  //
  static const auto cDriverGsckPort = GPIOA;
  static const auto cDriverGsckPin  = GPIO4;

  // number of TSC channels we're using
  //
  static const uint8_t cTscChannelCount = 6;

  // number of TSC channels we're using per group
  //
  static const uint8_t cTscChannelsPerGroup = 3;

  // touch sense controller I/O groups
  //
  static const auto cTscGroupControlBits = TSC_IOGCSR_GxE(3) | TSC_IOGCSR_GxE(6);

  // touch sensing controller I/O port
  //
  static const auto cTscPort  = GPIOB;
  static const auto cTscPortC = GPIOC;

  // pins used for sense capacitors
  //
  static const auto cTscSamplingCapPins = GPIO2 | GPIO14;

  // pins used for touch keys
  //
  static const auto cTscTouchKeyPins  = GPIO0 | GPIO1 | GPIO11 | GPIO12 | GPIO13;
  static const auto cTscTouchKeyPinsC = GPIO5;

  // touch sense controller I/O groups
  //
  static const uint32_t cTscChannelControlBits[] = { (1 << 8) | (1 << 20),
                                                     (1 << 9) | (1 << 21),
                                                     (1 << 10) | (1 << 22) };

  // touch sense controller I/O groups
  //
  static const uint32_t cTscSamplingControlBits = (1 << 11) | (1 << 23);

}

}
