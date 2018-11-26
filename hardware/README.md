# kbx's RGB Binary Clock

RGB Binary Clock PCB project files for KiCad (v4/v5)

## How do I get PCBs made for this?

Detailing the process is a bit outside of what I plan to discuss here. There are
 plenty of resources out around the web already that will do a better job of
 explaining this process than I can. That said, there's not much to it -- the
 files you need are here and all you need to do (more or less) is upload them
 to your PCB fabricator of choice and, after a few days, they'll ship them
 to you. I've used [OSHPark](http://oshpark.com) for the prototypes and I can
 assure you that they turn out fantastic (purple) boards.

As the vast majority of the components on this board are surface-mount parts,
 hot air is a fun approach for soldering it all together. If you go this route
 (and especially if you plan to build more than one of them), you'll likely want
 to have a stencil made to help with the placement of solder paste onto the
 board. I've used [OSHStencils](http://oshstencils.com) for this and they are
 very quick and do a wonderful job.

**Important**: The TLC5947 and TLC5951 LED driver ICs have a thermal relief (aka
 exposed) pad on their underside; this will not be possible to solder without a
 hot air soldering station or reflow oven. At low current/intensity levels, this
 shouldn't be a problem, but if you want your LEDs to be really bright, this
 could present an issue as the drivers will overheat quickly. Be careful!

## If I get boards made, how do I build it?

All the components are listed in the KiCad project files here; one can also find
 a bill of materials file named `kbxBinaryClock.bom.vX.csv` containing this
 list. For "unusual" parts (things that aren't resistors or capacitors) I've
 noted a suggested part number that should serve as a point of reference to help
 you find your own.

Before you order parts, be sure to read through the build notes below!

Once you have your boards and parts, there are at least a couple of different
 ways you can build the clock. Below are build notes.

## Build notes

### Before you begin...

...you need to determine:
1. Which version of the clock will you build? The answer should be "version 4"
 unless there is a specific reason you want/need an earlier version.
1. What will your time source be -- the STM32's RTC or the DS323x? The DS323x
 will provide the best accuracy. That said, these ICs add approximately $10 USD
 to the cost of the device. If you're purchasing them, be sure you're ordering
 the one that matches the version of the hardware you're planning on building!
1. What temperature sensor will you use -- the STM32's internal sensor, the
 DS323x's sensor, or a dedicated sensor? The DS1621/LM75/MCP9804/DS1722/LM74
 dedicated sensors will provide the best accuracy; however, if one of these ICs
 is not installed but a DS323x *is* installed, the app will use the DS323x's
 sensor. Note that, while the PCBs have footprints for more than one temperature
 sensor, only **one** of them may be installed at a time.
1. Solder-bridge/jumper settings. These are hardware version specific but *must*
 be set properly before you begin using the clock. More details are below...but
 don't forget about these!

Once you've settled on the items above, read on (below) for more detail and
 specifics regarding the build.

### Build details

#### External RTC:
**U6**

The external RTC--the DS3231 (hardware version 2.x) or the DS3234 (hardware
 version 3.x+)--adds a level of precision to the clock that is likely difficult
 to achieve with the STM32's internal RTC alone. These RTC ICs are designed to
 drift less than two minutes _per year_. It's possible that your STM32 hardware
 could end up being this accurate, but it's more likely that it will not be.
 Install this IC if you want a clock that is as accurate as is reasonably
 possible. Note that this IC also has a built-in temperature sensor (that's part
 of how it maintains its precision) that the application will use if it is
 installed and the other external temperature sensor (below) is _not_ installed.

If you install a DS3231/DS3234 on your clock, **do not install**:
 - Hardware v2.x: `C6`, `R8`
 - Hardware v3.x: `C6`, `R19`
 - Hardware v4.x: `C5`, `R3`

#### External temperature sensors:
**U7 and U8 (all hardware versions)**

A challenge with using the STM32's internal temperature sensor is that the
 sensor typically reads a few degrees higher than the temperature around the
 MCU itself. This is because the die, where the sensor is located, is usually
 just a little bit warmer then the package around it. An easy workaround is to
 just subtract a few degrees from the reading -- hence the calibration item
 you'll find in the application's menu structure. It works, but it requires
 someone to tweak it as each chip is likely to be a little different. Naturally,
 this takes away from its accuracy.

Installing an external temperature sensor is a great solution to this problem.
 The application will prefer an external sensor if one is installed; it first
 prefers one of the dedicated temperature sensors. If not installed, it will try
 to use the sensor integrated into the DS323x, should one be installed. If these
 all fail, it will use the STM32's internal sensor as a last resort.

**IMPORTANT NOTE: the PCBs (all hardware versions) have footprints for two
 external temperature sensor ICs. ONLY INSTALL ONE of them. DO NOT INSTALL
 BOTH.** If you install both, neither of them will work and you may cause damage
 to them or to other ICs on the board.

#### RS-485 transceiver:
**U9 (hardware v2.x and v3.x)**
**U6 (hardware v4.x)**

This part will enable the the second USART in the STM32 MCU to be used to
 communicate on an RS-485 bus. The original intention for this feature was to
 enable the device to receive a DMX-512 signal from an entertainment lighting
 console. With some development, it could also be used (for example) to
 synchronize a number of clocks with each other. This interface is more of a toy
 and has no impact on how the clock itself will function. You'll likely want to
 leave it off of the board unless you have a specific plan to use it for
 something.

#### Buzzer/beeper components:
**BZ1, D1, Q1, R1** (hardware v2.x and v3.x)
**BZ1, D2, Q2, R19** (hardware v4.x)

If you don't want your clock to be able to make any noise at all, it is safe to
 leave these parts off of your board. It'll be sad, but it'll work just fine.

#### Current drive control components:
**Q6 through Q11, R21 through R24, R26 through R29, R31 through R34,
 R36 and R37** (hardware v2.x);
**Q6 through Q11, R20 through R23, R25 through R28, R30 through R33,
 R35 and R36** (hardware v3.x)
(Not applicable to hardware v4.x)

These parts enable some software control over the constant-current mechanism in
 the TLC5947 PWM drivers. They enable, in a sense, a "master dimmer" control
 over the display as a whole. Their state can be defined through one of the menu
 options within the application. While not implemented just yet, future software
 versions might exercise them automatically, such as when the ambient light
 level changes.

#### Status LED components:
**LED25, LED26, Q2 through Q4, R2 through R7** (hardware v2.x and v3.x)
**LED25, LED26, Q3 through Q5, R20 through R22, R25 through R27** (hardware v4.x)

The status LED is not typically used during normal operation, however it can be
 enabled as an AM/PM indicator if/when the clock is set in 12-hour mode. It can
 also provide troubleshooting information if your clock doesn't seem to be
 working. That said, it's not necessary unless you like LEDs as much as I do.

#### Reset/Boot buttons:

The reset and boot buttons are of course not required, but it is convenient if
 you plan to do any development with this platform on your own.

### Other requirements

#### 32.768 kHz crystal installation and connection:

A footprint for a 32.768 kHz crystal exists on all of the PCBs. It is used to
 drive the STM32's internal RTC mechanism. This crystal is particularly
 important if the DS323x IC (U6) is **not** installed; in this case, it must be
 connected to the MCU by creating a solder bridge across the appropriate two
 pads on `SB1`. Failure to do so--and if the DS323x is not installed--means the
 clock won't run (although things otherwise may seem to work).

If the DS323x **is** installed, `SB1` may be bridged so as to connect the
 DS323x's 32.768 kHz signal output to the STM32's OSC32 input. This is not
 required but it is recommended so that this MCU input isn't left floating. In
 addition, if `SB1` is bridged in this manner, the 32.768 kHz crystal (and its
 drive capacitors) should **not** be installed.

#### Phototransistor series resistors:
 **Q5, RV1, R9 and R10** (hardware v2.x);
 **Q5, RV1, R8 and R9** (hardware v3.x)
 **Q1, RV1, R4 and R5** (hardware v4.x)

The phototransistor enables the application to determine the amount of ambient
 light around the clock so it can adjust the overall display intensity
 accordingly. There is a variable resistor in series with a fixed resistor which
 allows some manual adjustment over the sensitivity of this mechanism. There is
 also a single fixed resistor in parallel with this configuration. It is not
 necessary to install all of these, however it won't hurt anything if you do.
 If the specified phototransistor is used, a single 100K to 150K resistor
 installed on the larger component number's (`R5`, `R9` or `R10`, depending on
 your hardware version) pads will probably work well.

#### Status LED:

If installing a status LED, install only LED25 **or** LED26. Do not install
 both.

## After building...

You'll need to flash the firmware onto the STM32 MCU. Hardware version 4.x of
 the clock also has a USB interface that may be used for this purpose, in
 addition to the SWD connection (P1/P2) or the serial interface on J2/J4. Either
 way, you'll need an appropriate adapter to do so. You can find the source,
 compiled binary files and flashing instructions in the `src` folder one level
 up in the repo's hierarchy.

## Known Issues and Noteworthy Items

You may have noticed the "hefty" 2-amp power supply rating on the board. Why is
 it so big? Well, each LED is rated for 20 milliamps (mA)...but are three colors
 per physical LED, adding up to 60 mA each. Then there are 25 LEDs...which adds
 up to 1500 mA -- a lot of current! We need a little extra for the other parts
 of the clock, so a 2 amp supply covers that nicely. That said, under normal use
 as a clock, it's unlikely that *all* of the LEDs would ever be fully on and
 driven at a full 20 mA each; in this case, you can likely get away with a power
 supply with a lesser rating. Under DMX-512 (or some other) control, however, it
 is possible that this situation could occur, so the goal was to ensure that the
 board could handle it. With that in mind, however, **if you wish to drive all
 of the LEDs at their full rated current of 20 mA for more than a few seconds at
 a time, you need to ensure three things happen:**
* The exposed pad on the LED driver ICs must be properly soldered to the PCB
* Heatsinks should be glued (with appropriate thermal epoxy) to the driver ICs
* The PCB should be manufactured with a 2 oz copper weight

Damage to your board and/or the parts on it may occur if the heat is not
 sufficiently dealt with -- the drivers can become VERY HOT!

Speaking of heat, said heat from the LED drivers can skew the temperature
 reading from the sensor on the board. This becomes more noticeable as the LEDs
 become brighter. Version 4.x of the PCBs includes footprints for additional
 2.54mm pin headers which could be used for mounting the temperature sensor(s)
 off of the main board.

Due to the nature of how the TLC5947 drivers work, frequent display refreshes
 (such as when LEDs are fading between colors and/or intensities) sometimes
 cause some faint flickering or pulsing of the LEDs. Version 4.x of the hardware
 resolves this issue by using TLC5951 drivers which can be synchronized with
 display refreshes.

## Legal stuff and License

The circuit schematics and PCBs found here are licensed under the
 [Creative Commons Attribution-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-sa/4.0/).

![Creative Commons License badge](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

_Happy building!_
