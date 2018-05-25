# kbx's RGB Binary Clock

RGB Binary Clock firmware source files

## What's going on here?

Here you will find the C++ source code files for the RGB Binary Clock. It is
 modularized so that its various parts can be broken out and potentially re-used
 for other projects. It's not perfect but it works well and the plan is to
 continue to update and improve it over time.

The firmware implements all of the clock's functionality from the simple display
 of the time and date through color selection and management, alarms, and even
 DMX-512 control for the display LEDs. It could be easily modified to provide a
 variety of other capabilities if desired.

## More specifics

The code is written for the STM32F072 microcontroller used on the RGB Binary
 clock. With a little effort, it should be relatively easy to port it to another
 STM32Fx-series MCU. It relies on [libopencm3](http://libopencm3.org) as the
 underlying platform to interface with the MCU. If you wish to do some hacking
 with this platform, a nice guide can be found
 [here](https://github.com/libopencm3/libopencm3-examples#reuse) which explains
 how to clone libopencm3 for (re)use within your own (or this) project. Note
 that if you do so, it may be necessary to tweak some paths in the Makefile(s),
 but otherwise things should work without much trouble. You'll also (of course)
 need a (gcc) compiler to build the code base. (At the time of this writing,
 I've been building it with gcc on macOS High Sierra.)

The STM32F072 microcontroller is a rich device with a lot of peripherals. Many
 of these peripherals are leveraged in the code to enable it work as efficiently
 as possible. For posterity, here is a list of the peripherals used:

* ADC - the analog to digital converter, used for sensing light level from the
  attached phototransistor
* DMA - direct memory access controller, used to speedily move data to/from
  other I/O peripherals such as I2C, SPI, and USARTs
* FLASH controller - erases and writes settings to a dedicated block of FLASH so
  settings are retained across power failures and do not depend on the battery
* GPIO - because how else do you toggle pins on/off or read input signals?
* I2C - interfaces (via DMA) with the DS3231 and temperature sensor on v2 boards
* IWDG - the watchdog timer makes sure the MCU gets reset in the event of some
  catastrophic software crash/failure
* NVIC - none of this would be possible without interrupts.
* RTC - the real time clock is used if the DS323x is not installed. Since it's a
  clock, you need a clock...sooooo...
* SPI - this is the interface (via DMA) to the display/LED driver ICs. On v3+
  boards, it is also the interface to the DS3234 and other temperature sensors.
* Timers - Timers 1 and 2 are used to generate PWM for the status LEDs and for
  the buzzer. Timers 15 and 16 are used to supervise DMX-512 communication.
* TSC - the touch sensing controller is used to sense touches on the touch keys.
* USARTs - USART1 is a general serial interface and USART2 is connected to an
  RS-485 transceiver. This firmware enables a DMX-512 signal to be received on
  USART2 so that the LEDs can be individually controlled remotely.

## I want to hack. Where do I start?

`RGBBinaryClock.cpp` is where it all begins. This connects the various
 interrupts to the appropriate functions for processing them, initializes the
 hardware, initializes the main application, then passes control to the main
 application loop. See `Application.cpp` and `Hardware.cpp` and work your way
 through the rest from there.

Please don't hesitate to reach out with any questions or comments -- it is nice
 to hear feedback. Happy hacking!
