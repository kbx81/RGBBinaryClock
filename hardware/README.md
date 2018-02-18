# kbx's RGB Binary Clock

RGB Binary Clock PCB project files for KiCad

## How do I get PCBs made for this?

Detailing the process is a bit outside of what I plan to discuss here. There are
 plenty of resources out around the web already that will do a better job of
 explaining this process than I can. That said, there's not much to it -- the
 files you need are here and all you need to do (more or less) is upload them
 to your PCB fabricator of choice and, after a few days, they'll ship them
 to you. I've used [OSHPark](http://oshpark.com) for the prototypes and I can
 assure you that they turn out fantastic boards.

As the vast majority of the components on this board are surface-mount parts,
 hot air is a fun approach for soldering it all together. If you go this route,
 you'll likely want to have a stencil made to help with the placement of solder
 paste onto the board. I've used [OSHStencils](http://oshstencils.com) for this.

## If I get boards made, how do I build it?

All the components are listed in the KiCad project files here; there is also a
 `csv` file containing the same list. For "unusual" parts (things that aren't
 resistors or capacitors) I've noted a suggested part number that should serve
 as a point of reference to help you find your own.

Once you have your boards and parts, there are at least a couple of different
 ways you can actually build the clock. Below are build notes.

## Build notes

### Optional parts

Before building, you should consider just how fancy you want your new binary
 clock to be. There are a few optional parts that you may choose to not place
 on the PCB if you are interested in a slightly less expensive build and are
 willing to sacrifice some degree of accuracy and/or functionality.

#### External RTC

**U6**

The external RTC--the DS3231 (hardware version 2.x) or the DS3234 (hardware
 version 3.x)--adds a level of precision to the clock that is likely difficult
 to achieve with the STM32's internal RTC alone. These RTC ICs are designed to
 drift less than two minutes _per year_. It's possible that your STM32 hardware
 could end up being this accurate, but it's more likely that it won't be.
 Install this IC if you want a clock that is as accurate as is reasonably
 possible. Note that this IC also has a built-in temperature sensor (that's part
 of how it maintains its precision) that the application will use if it is
 installed and the other external temperature sensor (below) is _not_ installed.

`C6` as well as `R8` (hardware v2.x) or `R19` (hardware v3.x) should not be
 installed if the DS323x is installed.

#### External temperature sensor

**U7 (both hardware versions), U8 (hardware v3)**

A challenge with using the STM32's internal temperature sensor is that the
 sensor typically reads a few degrees higher than the temperature around the
 MCU itself. This is because the die, where the sensor is located, is usually
 just a little bit warmer then the package around it. An easy workaround is to
 just subtract a few degrees from the reading -- hence the calibration item
 you'll find in the application's menu structure. It works, but it requires
 someone to tweak it as each chip is likely to be a little different. Naturally,
 this takes away from its accuracy.

Installing an external temperature sensor is a great solution to this problem.
 If present, the application will read the temperature from this sensor instead
 of the internal one (or the one in the DS323x if it happens to be installed, as
 well).

**IMPORTANT NOTE: Hardware version 3 has footprints for two external temperature
 sensor ICs. ONLY INSTALL ONE of them. DO NOT INSTALL BOTH.** If you install
 both, neither of them will work and you may cause damage to them or to other
 ICs on the board.

#### RS-485 transceiver

**U8 (hardware v2), U9 (hardware v3)**

This part will enable the the second USART in the STM32 MCU to be used to
 communicate on an RS-485 bus. The original intention for this feature was to
 enable the device to receive a DMX-512 signal from an entertainment lighting
 console. With some development, it could also be used (for example) to
 synchronize a number of clocks with each other. This interface is more of a toy
 and has no impact on how the clock itself will function. You'll likely want to
 leave it off of the board unless you have a specific plan to use it for
 something.

#### Buzzer/beeper components

**BZ1, D1, Q1, R1**

If you don't want your clock to be able to make any noise at all, it is safe to
 leave these parts off of your board. It'll be sad, but it'll work just fine.

#### Current drive control components

**Q6 through Q11, R21 through R24, R26 through R29, R31 through R34,
 R37 and R37** (hardware v2.x)

**Q6 through Q11, R20 through R23, R25 through R28, R30 through R33,
 R35 and R36** (hardware v3.x)

These parts enable some software control over the constant-current mechanism in
 the TLC5947 PWM drivers. They enable, in a sense, a "master dimmer" control
 over the display as a whole. Their state can be defined through one of the menu
 options within the application. While not implemented just yet, future software
 versions might exercise them automatically, such as when the ambient light
 level changes.

#### Status LED components

**LED25, LED26, Q2 through Q4, R2 through R7**

The status LED is not typically used during normal operation, however it can be
 enabled as an AM/PM indicator if/when the clock is set in 12-hour mode. It can
 also provide troubleshooting information if your clock doesn't seem to be
 working. That said, it's not necessary unless you like LEDs as much as I do.

#### Reset button

The reset button is of course not required, but it is convenient if you plan to
 do any development with this platform on your own.

### Other requirements

#### Y1 installation and connection

`Y1` is a 32.768 kHz crystal. It is used to drive the STM32's internal RTC
 mechanism. This crystal is particularly important if the DS323x IC (U6) is
 **not** installed; in this case, it must be connected to the MCU by creating a
 solder bridge across the appropriate two pads on `SB1`. Failure to do so--and
 if the DS323x is not installed--means the clock won't run (although things
 otherwise may seem to work).

If the DS323x **is** installed, `SB1` may be bridged so as to connect the
 DS323x's 32.768 kHz signal output to the STM32's OSC32 input. This is not
 required but it is recommended so that this MCU input isn't left floating. In
 addition, if `SB1` is bridged in this manner, `Y1` (and its drive capacitors,
 `C1` and `C2`) should **not** be installed.

#### Q5 series resistors

 **RV1, R9 and R10** (hardware v2.x)
 **RV1, R8 and R9** (hardware v3.x)

`Q5` is the phototransistor that enables the application to determine the amount
 of ambient light around the clock so it can adjust the display intensity
 accordingly. There is a variable resistor in series with a fixed resistor which
 allows some manual adjustment over the sensitivity of this mechanism. There is
 also a single fixed resistor in parallel with this configuration. It is not
 necessary to install all of these, however it won't hurt anything if you do.
 If the specified phototransistor is used, a single 100K to 150K resistor
 installed on the larger component number's (`R9` or `R10`, depending on your
 hardware version) pads will probably work well.

#### Status LED

If installing a status LED, install only LED25 **or** LED26. Do not install
 both.

## After building...

You'll need to flash the firmware onto the STM32 MCU. This can be done via the
 SWD connection (P1) or via the serial interface on J2. Either way, you'll need
 an appropriate adapter to do so. You can find the source, compiled binary files
 and flashing instructions in the `src` folder one level up in the repo's
 hierarchy.
