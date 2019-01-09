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
**U9** (hardware v2.x and v3.x)\
**U6** (hardware v4.x)

This part will enable the the second USART in the STM32 MCU to be used to
 communicate on an RS-485 bus. The original intention for this feature was to
 enable the device to receive a DMX-512 signal from an entertainment lighting
 console. With some development, it could also be used (for example) to
 synchronize a number of clocks with each other. This interface is more of a toy
 and has no impact on how the clock itself will function. You'll likely want to
 leave it off of the board unless you have a specific plan to use it for
 something.

#### Buzzer/beeper components:
**BZ1, D1, Q1, R1** (hardware v2.x and v3.x)\
**BZ1, D2, Q2, R19** (hardware v4.x)

If you don't want your clock to be able to make any noise at all, it is safe to
 leave these parts off of your board. It'll be sad, but it'll work just fine.

#### Current drive control components:
**Q6 through Q11, R21 through R24, R26 through R29, R31 through R34,
 R36 and R37** (hardware v2.x)\
**Q6 through Q11, R20 through R23, R25 through R28, R30 through R33,
 R35 and R36** (hardware v3.x)\
(Not applicable to hardware v4.x)

These parts enable some software control over the constant-current mechanism in
 the TLC5947 PWM drivers. They enable, in a sense, a "master dimmer" control
 over the display as a whole. Their state can be defined through one of the menu
 options within the application. While not implemented just yet, future software
 versions might exercise them automatically, such as when the ambient light
 level changes.

#### Status LED components:
**LED25, LED26, Q2 through Q4, R2 through R7** (hardware v2.x and v3.x)\
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
 important if the DS323x IC is **not** installed; in this case, it must be
 connected to the MCU by creating a solder bridge across the appropriate two
 pads on `SB1`. Failure to do so--and if the DS323x is not installed--means the
 clock won't run (although things otherwise may seem to work).

If the DS323x **is** installed, `SB1` may be bridged so as to connect the
 DS323x's 32.768 kHz signal output to the STM32's OSC32 input. This is not
 required but it is recommended so that this MCU input isn't left floating. In
 addition, if `SB1` is bridged in this manner, the 32.768 kHz crystal (and its
 drive capacitors) should **not** be installed.

#### Phototransistor series resistors:
 **Q5, RV1, R9 and R10** (hardware v2.x)\
 **Q5, RV1, R8 and R9** (hardware v3.x)\
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

### Solder Bridge Jumpers

#### 32.768 kHz Oscillator jumper
**SB1**

This jumper determines if the 32.768 kHz oscillator or the DS323x's 32.768 kHz
 output pin is connected to the STM32's OSC32_IN pin. If a DS323x is **not**
 installed on the board, it must be bridged so that the crystal's pin is
 connected to the OSC32_IN pin. Otherwise, it's best to bridge it to the
 DS323x's 32.768 kHz output pin. It can be determined with a close look at the
 PCB which position is which.

#### Reset Supervisor jumper
**SB2** (hardware v2.x and v3.x)\
**SB4** (hardware v4.x)

This jumper determines if the DS323x's reset supervisor output pin is connected
 to the STM32's NRST (not-reset) pin. This should be connected if the DS323x is
 installed, however it sometimes causes issues with flashing the MCU. If you
 installed a DS323x and are having difficulty flashing the MCU, try
 disconnecting this jumper until the flashing process is complete. Don't forget
 to reconnect it, though!

#### USART 1/USB VBUS jumpers
**SB2, SB3** (hardware v4.x)

These jumpers are on version 4+ boards and allow some configuration of which
 USART1 pin on the MCU is connected to the six-pin serial header. Because the
 processor expects USB VBUS on PA9, if you plan to use the USB port, SB2 should
 be bridged and SB3 should be bridged to put PB6 on the on the six-pin header.
 If you do not want/need to use the USB port and wish to flash the MCU via the
 six-pin header, SB2 must be left **open** and SB3 should be bridged to put PA9
 on the six-pin header.

#### RS-485 Terminator jumper
**SB5** (hardware v4.x)

Bridge this jumper to connect the terminating resistor across the RS-485
 transceiver's input/output pins. This is useful if the board is at the end of
 a RS-485 link.

## Before powering it on for the first time...

WAIT! you didn't plug it in yet, did you? Let's do some pre-flight checks to get
 ready to bring up your board!

It's important that we go over the board with a DMM (or at least a continuity
 tester) and check for any nasty shorts. Solder bridges and poor part placement
 are the typical causes of this and it's not uncommon to have them, particularly
 with so many surface-mount parts. We don't want our brand-new clock to let out
 the magic smoke before we even get to use it, do we? :)

Generally, it will suffice to check a few key areas for shorts. Since these are
 surface-mount ICs, it's not uncommon to have solder bridges between some of the
 pins after you've soldered it all together. Don't worry, we can get away
 without checking _all_ of them, but we _do_ need to check a few. Let's make a
 list so we can check them off:
* **Check for shorts between the output of the LDO and ground.** Put one of your
 DMM probes on the power connector's ground pin and the other on the LDO's
 heatsink (the top fin soldered to the board -- this is its 3.3-volt output). If
 you see they're shorted, you'll have to go hunting for the short. Start by
 checking the power pins around the STM32 processor. All of its Vcc and ground
 pins are right next to each other so this is the best place to look first.
* **Check for shorts between the LED cathode pins and ground.** Again, put one
 of your DMM probes on the power connector's ground pin and drag the other probe
 across each of the pins of the LEDs (this also serves as a chance to check for
 a short directly across the power input -- touch any of the anode pins to do
 so). If there's a short, touch-up the pins on the TLC59xx drivers. It might
 even be necessary to lift the driver IC completely off of the board (you have a
 hot air soldering station, right?). Due to the exposed/thermal pad on the
 bottom of the TLC59xx LED drivers, it's possible to have a solder bridge
 between one of the LED pins and ground. If this happens, the affected LED(s)
 will pop/fry/smoke/burn out immediately upon applying power to the board.
* **Check for shorts between the TLC59xx IREF pin and ground.** These pins are
 also adjacent and, if IREF is shorted to ground, too much current will flow
 through the LEDs (they'll be super bright and burn out quickly). This situation
 also increases the power dissipation of the TLC59xx drivers so they'll heat up
 unnecessarily. This also affects the power draw on the LDO so it'll get
 unnecessarily hot, too!
* **Check that the solder bridge "jumpers" are set correctly.** You didn't
 forget to bridge these as appropriate, did you? :) See the "Jumpers" section
 above for information on how they should be set.

Once you've confirmed everything above looks good, it should be reasonably safe
 to proceed to the next step -- flashing the firmware onto the STM32 processor!
 After we do this, we'll be able to more effectively identify any other
 soldering-related issues that could be affecting the clock's operation.

After you power on your board for the first time--and if nothing blows
 up--you'll need to flash the firmware onto the STM32 MCU. Hardware version 4.x
 of the clock also has a USB interface that may be used for this purpose, in
 addition to the SWD connection (P1/P2) or the serial interface on J2/J4. Either
 way, you'll need an appropriate adapter to do so. You can find the source,
 compiled binary files and flashing instructions in the `src` folder one level
 up in the repo's hierarchy. With the right tools in hand, this can be done in a
 couple of minutes.

If you're having trouble flashing the MCU, please see the next section.

## Troubleshooting

What's that? You couldn't flash the MCU? Or maybe it isn't working perfectly
 after you flashed the MCU? Uh oh...

As I've built...a few...of these to date, here is a list of common issues I've
 stumbled across as well as some notes on how to fix them.

If you're having trouble flashing the MCU, check these items first:
* If a DS323x is installed, is the reset supervisor jumper bridged? If so, this
 may be holding the STM32 in a reset state too long and your programmer is
 giving up. Open this jumper and try again.
* If there is no DS323x installed and/or the jumper is already open, check the
 data lines  between the port you're using (six-pin serial, USB, or SWD) and the
 processor. Perhaps there is another nasty solder bridge somewhere that's
 shorting one or more of the lines to Vcc, ground, or another adjacent pin. If
 you're using the USB interface on a version 4+ board, take a close look at D1
 and the physical USB port itself, as well.

If you flashed the MCU and it's now doing...something...but it doesn't seem
 right, here are some more common symptoms and how to fix them:

* **Status LED blinks white/gray but the main display is blank or flickers
 erratically.** This is indicative of some soldering problem on the SPI1 pins
 of the MCU or on one or more of the TLC59xx drivers. Take a close look at these
 pins and rework them as necessary.
* **One or more of the touch keys don't work.** Check the 1K resistors that sit
 between the touch keys and the MCU as well as the 47 nF sense capacitors.
 Again, also check the relevant pins on the MCU itself for shorts to adjacent
 pins.
* **It keeps making a ticking sound.** There's a problem with one or more of the
 touch keys -- see the item above.
* **The status LED alternates red and yellow.** You did not install a DS323x (or
 maybe you did and it's not working) and the STM32's RTC/LSE oscillator did not
 start within the expected amount of time. Check that `SB1` is bridged correctly
 and that the 32.768 kHz crystal and its bypass capacitors are installed and
 soldered correctly.
* **One or more of the LEDs are weird/mismatched colors.** This indicates that
 one or more of the LED pins on the TLC59xx drivers aren't soldered well. It
 could also mean that you have (a) bad LED(s) (maybe you overheated them or
 heated them for too long while soldering). Rework the relevant pins on the
 TLC59xx ICs and (hopefully) your LEDs will look normal afterwards. If not, you
 may want to try swapping out the affected LED(s).
* **One or more blocks of LEDs is brighter than the other block(s) of LEDs
 and/or one or more of the TLC59xx drivers is HOT along with the LDO.** Check
 the IREF pin on the (hot) driver below the block of LEDs that appears to be
 brighter than the others. It's probably shorted to ground.

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
 resolves this issue by using TLC5951 drivers. These have a clock input to the
 PWM engine enabling writes to the data latches to be synchronized with PWM
 cycles. The result is smooth fading of the LEDs at all intensities with some
 additional benefits, as well!

## Legal stuff and License

The circuit schematics and PCBs found here are licensed under the
 [Creative Commons Attribution-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-sa/4.0/).

![Creative Commons License badge](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

_Happy building!_
