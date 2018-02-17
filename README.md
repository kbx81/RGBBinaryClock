# kbx's RGB Binary Clock

RGB Binary Clock project files - PCB and source

_It's probably more than what you're thinking..._

## Another binary clock? Why?

The world probably doesn't need another (binary) clock. There are plenty of them
--all different shapes and sizes--so why build another one?

This project was started with two main goals in mind: I wanted a "simple"
 project I could use to help me get back into the world of electronics. The
 second goal I had in mind was to build up my comfort level with C and C++ -- I
 knew the language(s) but was never terribly comfortable with them. Building
 this helped me to achieve both of those goals and then some -- and I am quite
 happy with the result!

_But why a clock?_

It's fun to make LEDs blink. It's fun to play with microcontrollers. It's fun
 to build stuff. But a blinking LED or two or three isn't very meaningful. Nor
 is a nice microcontroller that just toggles a GPIO pin. If you're going to
 build something, why not make it at least modestly useful? And why not build it
 in a unique way that's neat to look at and/or watch?

The idea for this came from an old electronics kit I put together probably
 around twenty or so years ago (and it still works today!). I liked the kit but
 it was somehow lacking. Probably the most obvious problem was that, in a dark
 bedroom, the display was too bright. It was a simple, 12-hour-only binary
 clock, built on 4000-series CMOS logic. Now, there's nothing wrong with that,
 but...it's 2018 now. Let's take it up a notch or ten.

## How do you take a binary clock up a notch (or ten)?

First, you can add some color. A lot of color. In fact, you can pick the colors
 you want it to use...up to eight times over. This binary clock has the ability
 to gradually shift colors over the course of each day. Bits can even fade in
 and out as they change. It's all about the eye candy in this regard.

Second, it can do more than just tell the time -- it'll tell you the date and
 temperature, too! There is even a timer/counter mode. What's more, you can
 choose the format for it all: the clock can display in a 12 or 24 hour format,
 the temperature can display in degrees Celsius or degrees Fahrenheit, and (most
 importantly) it can display all of these values in either binary-coded decimal
 (BCD) or good old-fashioned binary. You get to choose the formats you prefer
 and they can be changed easily at any time.

Next, the display brightness issue had to be addressed -- it has a
 phototransistor which is used to determine the amount of ambient light around
 it and the display will dim as the light level around it diminishes. This is
 great if you want to keep it near you at night while you sleep.

Another cool bit is that it uses capacitive touch keys for buttons; the kit it
 is based on had mechanical, fixed-function buttons. This also adds a little
 extra "coolness" to the clock, too.

A CR2032 coin-cell battery back-up can be installed to keep the time valid in
 the event that the board loses power.

Finally--and one could argue that no clock is complete without one--it has an
 alarm! The alarm can be set to beep at any of the eight times the user sets.
 There is also an hourly chime that one can enable which will beep out each hour
 in binary using high and/or low pitch tones...so you can hear the time when
 you're in another room!

## Great, but what makes it tick?

The "brain" is an STM32F072 microcontroller. This MCU alone has everything
 that's necessary to have a functional time clock -- temperature sensor
 included. Still, it might not be quite as accurate as some of us would like.
 For those folks, the PCB has footprints for a Maxim DS3231 "extremely accurate"
 temperature-compensated crystal (TCXO) I2C real time clock IC as well as an
 LM75 (or compatible) I2C temperature sensor. Why both? The DS3231 is somewhat
 expensive and it's possible that one might want more accurate temperature
 sensing abilities but isn't as concerned with the accuracy of the clock.

Beyond the MCU itself, the board has 25 RGB LEDs on it; 24 of them form the main
 display and they are connected to TLC5947 constant-current PWM drivers. The MCU
 uses the SPI1 to communicate with these drivers. The remaining RGB LED is used
 as a "status" LED and it is connected (through FET drivers) to GPIO pins on the
 MCU. These pins double as timer output channels, meaning they can also generate
 a PWM signal, enabling the dimming of the status LED elements, as well.

The beeper is connected (also through a FET driver) to yet another GPIO pin
 that doubles as a timer output channel; this enables the beeper to generate a
 wide range of tones -- it could potentially play a tune!

The phototransistor is connected to the MCU's ADC channel zero.

Two USARTs are exposed via pin headers on the right side of the board: USART1
 is brought out on a standard six pin header as is commonly found on many
 devices while USART2 is connected to an RS-485 line driver enabling
 communication on an RS-485 bus. (It's not implemented at the time of this
 writing but the idea is to enable it to receive a DMX-512 signal so the LEDs
 can be controlled from a standard entertainment lighting console!)

## How do I get or build one?

In this repository you'll find everything needed to put one together. It is
 divided into two major parts: hardware and software (source). The `hardware`
 directory contains the [KiCad](http://kicad.org) project files used to create
 the printed circuit board. The `src` directory contains the source code needed
 to compile and run the application on the microcontroller. It is built on top
 of [libopencm3](http://libopencm3.org).

Additional details regarding the hardware and software can be found in the
 `README.md` files located in each respective directory.

 _That's all for now...more information will be coming soon!_
