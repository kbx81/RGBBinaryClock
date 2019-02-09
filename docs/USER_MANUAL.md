# kbx's RGB Binary Clock

_So...I built one (or someone gave me one), loaded the firmware and...now what?_

Congratulations! Read on...

## Getting started with your RGB Binary Clock

After confirming everything is working, using the clock is pretty simple! Here's
 how it works:

When powered on, the clock is either set or it is not set (such as if the
 battery/capacitor drained or this is the first time you've powered it on).
* If the clock is set, it will resume displaying the time (and date and
  temperature, if the "Default to Toggle mode" option is enabled. More on this
  later on).
* If the clock is not set, the least-significant bits will blink at .5 Hz
  in white on a dark-gray "background".

If you see the latter (flashing LSbs), you'll need to set the time and date!
 But before we do that...

### How do I read the display?

The main display consists of 24 RGB LEDs; there is also a "status" RGB LED.
 You'll quickly notice that the display is divided into three "blocks". For the
 sake of this guide, let's refer to them like this:
```
X T  P L  H D
W S  O K  G C
V R  N J  F B
U Q  M I  E A
```

#### For the experienced binary enthusiast

In BCD mode:
```
D, H, L, P, T, X = 8 (2^3) MSbs, top row
C, G, K, O, S, W = 4 (2^2)
B, F, J, N, R, V = 2 (2^1)
A, E, I, M, Q, U = 1 (2^0) LSbs, bottom row
```
In binary mode:
```
H, P, X = 128 (2^7) MSbs
G, O, W = 64  (2^6)
F, N, V = 32  (2^5)
E, M, U = 16  (2^4)
D, L, X = 8   (2^3)
C, K, S = 4   (2^2)
B, J, R = 2   (2^1)
A, I, Q = 1   (2^0) LSbs
```

You're all set! Skip to the next section to begin learning how to use the clock.

#### The longer explanation...

As with a not-binary clock, the display shows you numbers. Hours, minutes, and
 seconds...or year, month, day (see footnote below)...you get the idea and
 (hopefully) are starting to see the connection. That said, how do we read this
 one?

The clock has the ability to display numbers in two different ways: in binary,
 or in binary-coded decimal (BCD, which is the default). (Which format it uses
 can be easily changed at any time -- more on this later.) It is very important
 to distinguish between them as it directly affects how information on the
 display is interpreted.

Since it is the default, let's discuss the BCD mode first. In BCD mode, we're
 looking at decimal numbers (0 through 9) encoded in binary. That is, for each
 column of LEDs in the display, possible values are 0 to 9. As there are six
 columns, and because of the "decimal" part in "binary-coded decimal" we have
 (from left to right) tens of hours/years (U-X), ones of hours/years (Q-T), tens
 of minutes/months (M-P), ones of minutes/months (I-L), tens of seconds/days
 (E-H), ones of seconds/days (A-D). With these identified, all we have left is
 to identify the binary place values -- take a look at the table above.

In the binary display mode, each of the three "blocks" of LEDs forms a single
 byte. Since a byte consists of eight bits, and because each bit can be either a
 one or a zero, we have 2^8 or 256 possible values we can display in each block
 of LEDs, which translates to values from 0 to 255. This works out perfectly if
 we consider what the clock displays -- hours, minutes, seconds, years, months,
 days, and (room) temperature (whether degrees C or degrees F). None of these
 values dwell outside of the range of 0 to 255 (well, assuming we display the
 year as two digits). That said, each block of LEDs (A-H, I-P, Q-X) corresponds
 directly to the eight bits in a byte. The "lowest" letter (A, I, Q) in each
 block is the least-significant bit and the "highest" letter (H, P, X) in each
 block is the most-significant bit. See the table above for their exact place
 values.

If at this point you're thinking "I'm lost", you'll want to do some additional
 reading/research ([Google](http://google.com) is your friend) on the binary
 number system -- it's a bit out of scope for this document but will prove
 helpful (even necessary) as we move forward.

### I can read it! Now what?

If you've powered it on and are looking at blinking LSb and status LEDs, you'll
 first need to set the time and date. To do so, start by touching the `E` touch
 key. This will exit the time/date/temperature view and enter into the main menu
 view.

#### The Main Menu view

You can identify the main menu view as you'll see white-on-gray LEDs
 (these are the default colors, although they can be changed -- more on this
 later) with the selected menu item number displayed on the left-most block of
 LEDs. Press the `U` key to increase or the `D` key to decrease the selected
 menu item number as indicated by the LEDs. To make your selection, touch the
 `E` key again to enter into the selected mode/menu item. (Perhaps you are
 starting to build the connection that `E` means "Enter"/"Exit".)

#### Setting the time and date

If, when you started, the time and date were not set, entering the main menu
 view will automatically select option 5, the "set time" function. If option 5
 is not selected, use the `U` and/or `D` keys to select it. Touch the `E` key to
 enter into the set time/date view to set the time.

Once you've entered into mode 5, you'll see the set time/date view. This is the
 standard view used to set times and dates. By default, the display uses green
 for bits with a value of `0` and red for bits with a value of `1` (although,
 again, these colors can be changed -- more on this later). In this view, the
 touch keys work as follows:

* `U` increases the selected value
* `D` decreases the selected value
* `C` moves the selection left
* `B` moves the selection right
* `A` applies the date/time as set
* `E` exits to the main menu

Use the keys to adjust the values to the correct time and touch the `A` key to
 commit the time shown on the display to the clock's hardware. When you're done,
 touch the `E` key to exit back to the main menu.

**Important:** You must touch the `A` key to save the time/date as set! If you
 do not, the selected values will be lost upon exiting the set time/date view.

Once the time has been set, the date must be set. Upon exiting mode 5 (set
 time), mode 6 will be automatically selected if the date has not been set
 (that is, if the year is equal to zero, which...yeah...). In any event, mode 6
 is the "set date" mode and, once it is selected in the main menu, touch the `E`
 key again to enter that mode to set the date.

Setting the date works exactly the same way as setting the time does in terms of
 the user interface. Note that, by default, the display shows **year**,
 **month**, **day** (see footnote below). Use the keys as described above to
 make the appropriate selections and then press the `A` key to apply them. The
 `E` key exits back to the main menu view.

 #### I set my clock! What's next?

 Congratulations! You've now got a working, set clock! At this point, use the
  main menu to select either mode 1 (fixed time/date/temperature display) or
  mode 2 (auto-toggle time/date/temperature displays) to see the time, date,
  and/or temperature.

At this point, we're left with...

## Advanced topics and settings

Your RGB Binary Clock can do more than just show you the time, date, and
 temperature in binary or BCD. Let's dig in and take a closer look!

### Views

The RGB Binary Clock has a number of configuration options that may be changed,
 allowing some customization of the clock's behavior. If you've just gone
 through the process of setting the time and date, you've already begun using
 some of its simple menu system and "views".

Configuration options/settings are changed by using a number of different views.
 These views provide a way to access the various settings so that they may be
 modified. Each setting is really nothing more than a number, but this number
 determines something: the volume of the beeper, the intensity of a color on the
 display, or the hour in which DST takes effect, as some examples. The various
 views simply display this information in a way that makes sense while providing
 a means by which to modify the settings.

Entering into any given mode activates a view. Each view presents information on
 the display a little differently. In addition, the touch keys are used somewhat
 differently in each view. Let's describe the views now, as, later on when we
 discuss the various modes, we'll refer back to the views to describe the
 expected behavior (what you'll see on the display, what the touch keys will do,
 etc.) upon entering into any given mode.

#### Main Menu View

If you've gone through the process to set the time and date, you've already seen
 this view. You can identify the main menu view as you'll see (by default, at
 least) white-on-gray LEDs with the selected menu item number displayed on the
 left-most block of LEDs. The touch keys are used as follows:
* `U` increases the selected mode number
* `D` decreases the selected mode number
* `C` enters Time/Date/Temperature view in toggle mode (aka mode 2)
* `B` enters Time/Date/Temperature view in fixed mode (aka mode 1)
* `E` enters into the selected mode

#### Time/Date/Temperature View

This is the view that--you guessed it--displays the time, date, and temperature.
 This same view is used whether the clock is in mode 1 (fixed display) or mode 2
 (toggle/rotate display). This is the view in which your RGB Binary Clock will
 spend the vast majority of its time. In this view, the touch keys are used as
 follows:
* `U` increases the display intensity (if automatic adjustments are disabled)
* `D` decreases the display intensity (if automatic adjustments are disabled)
* `C` displays the temperature
* `B` displays the date
* `A` toggles display of time (hours, minutes, seconds) and seconds-since-midnight
* `E` exits to the main menu

#### Timer/Counter View

This is the view that displays the timer/counter. In this view, the touch keys
 work as follows:
* When the timer/counter is stopped:
  * `U` increases the timer/counter value
  * `D` decreases the timer/counter value
* When the timer/counter is running:
  * `U` counts up at next start
  * `D` counts down at next start
* `C` resets the timer/counter
* `B` stops the timer/counter
* `A` starts the timer/counter
* `E` exits to the main menu

#### DMX-512 View

This view displays data being sent to the clock via DMX-512. If no DMX-512
 signal is present on the RS-485 serial interface, this view does nothing and
 will not activate. See the DMX-512 parameters document for more information.

#### Set Time/Date View

If you've already set your clock, you've seen this view; it was also described
 above. This is the view used to set times and dates. By default, the display
 uses green for bits with a value of `0` and red for bits with a value of `1`.
 The existing values are shown when the view is entered. One block of LEDs will
 be "highlighted" at a time and the value on the highlighted block is what will
 be modified by the `U` and `D` touch keys. In this view, the touch keys work as
 follows:
* `U` increases the selected value
* `D` decreases the selected value
* `C` moves the selection left
* `B` moves the selection right
* `A` applies the date/time as set
* `E` exits to the main menu

#### Set Bits View

This view is used to modify settings that are either on/enabled/`1` or
 off/disabled/`0`. By default, the display uses green for bits with a value of
 `0` and red for bits with a value of `1`. The existing settings are shown when
 the view is entered. One LED will be "highlighted" at a time. The touch keys
 can be used to toggle the various settings on and off as follows:
* `U` selects the next bit
* `D` selects the previous bit
* `C` sets/turns on the bit
* `B` clears/turns off the bit
* `A` applies (saves) the settings
* `E` exits to the main menu

#### Set Value View

This view allows adjusting of various settings values. The existing value is
 shown when the view is entered and it appears on the right side of the display.
 By default, the display uses green for bits with a value of `0` and red for
 bits with a value of `1`. The touch keys work as follows:
* `U` increases the displayed value
* `D` decreases the displayed value
* `C` sets the displayed value to the maximum allowed value for the setting
* `B` sets the displayed value to the minimum allowed value for the setting
* `A` applies (saves) the selected value
* `E` exits to the main menu

#### Set Colors View

Any given color as it appears on the RGB LEDs consists of a mix of red, green,
 and blue elements. As bits have two possible states (`0` or `1`), each color
 setting that the RGB Binary Clock stores consists of two colors -- one for `0`
 bits and one for `1` bits.

This view is used to modify the color settings for the RGB Binary Clock's
 various displays/views. It allows the modification of two colors
 simultaneously: the `0` color and the `1` color. The display is divided into
 two parts: the left and center LED blocks display the current value of the
 selected color element and the right block of LEDs displays a preview of the
 resulting colors based on the red, green, and blue values chosen.

On the left and center LED blocks, you'll see one of seven possible displays:
1. Red on Gray (`1` color)
1. Green on Gray (`1` color)
1. Blue on Gray (`1` color)
1. White on Red (`0` color)
1. White on Green (`0` color)
1. White on Blue (`0` color)
1. White on Gray (increment)

Items 1 through 3 indicate that the `1` color (red, green, or blue) is selected
 for adjustment; items 4 through 6 indicate that the `0` color is selected for
 adjustment. The last item indicates the current value of the increment to be
 used as any given color is adjusted; it appears when the `B` key is touched.
 When the `U`, `D`, or `C` keys are touched, the display will revert back to the
 selected color element (one of items 1 through 6 above).

To summarize, the touch keys work as follows:
* `U` increases the selected color value by the selected increment
* `D` decreases the selected color value by the selected increment
* `C` changes the selected color element (see items 1 through 6 above)
* `B` changes the selected increment (1, 16, or 256)
* `A` applies (saves) the selected value
* `E` exits to the main menu

### Time Slots

The RGB Binary Clock has the ability to store up to eight "times of
 significance" -- you can think of it as an alarm clock with eight different
 alarm times that you can set. Moving forward, we'll refer to each of these
 times as a "time slot".

Each of these eight time slots can be configured to trigger any (combination) of
 three events:
1. An audible alarm
1. A blinking display
1. A display color change

The timer/counter and external alarm inputs (hardware version 4+) can also
 trigger the audible alarm or display blinking.

The first is probably self-explanatory -- when the time matches the time that
 was set into the time slot, the alarm will sound. It's just like an alarm clock.

The second is similar to the audible alarm, but instead of beeping, it'll cause
 the display to switch to full brightness and blink at 1 Hz.

The third is a bit more unique -- it enables a new set of display colors to be
 activated when the time set into the time slot is reached. The "Set Colors
 View" described above is used to modify the colors assigned to any of these
 eight time slots as well as the date, temperature, timer/counter, main menu,
 and configuration/settings (Set Time/Date, Set Bits, and Set Value) views.

### Other modes

At this point, you should have a feel for how to select a given mode by using
 the main menu view. (If not, scroll up and (re)read the Main Menu view section
 above.) On the back of the PCB you'll find a listing of all of the
 clock's various operating modes; the numbers seen in that list correspond
 directly to what you can select in the main menu view. Let's go through them
 now.

#### Modes 1 and 2: Time/Date/Temperature view

These modes were mentioned briefly above, but left out some detail.

The only difference between these two modes is that mode 2 will automatically
 rotate the display between the time, date, and temperature. That aside, they
 behave the same, as they both use the Time/Date/Temperature view as described
 above.

In mode 2, it is possible to adjust how long the time, date, and temperature
 each appear. The duration for each is configured by adjusting the values set in
 modes 20, 21, and 22. The values are in seconds and the defaults are 24 (time),
 3 (date), and 3 (temperature).

#### Mode 3: Timer/Counter

Mode three allows the use of the clock as a timer or counter. It can count up or
 down in seconds. See the Timer/Counter View section above.

An alarm can be triggered when the timer reaches either zero (if counting down)
 or the reset value (if counting up). The timer/counter reset value is
 determined by the value set in mode 7. This value is applied when the
 timer/counter is reset and the timer is set to count down; if set to count up,
 resetting the timer sets it to zero.

#### Mode 4: DMX-512 view

This mode displays data being sent to the clock via DMX-512. If no DMX-512
 signal is present on the RS-485 serial interface, this mode does nothing and
 cannot be selected. See the DMX-512 parameters document for more information.

#### Modes 5 and 6: Set time/date view

View: Set Time/Date View

These modes were described in the "Getting Started" and "Set Time/Date View"
 sections above.

#### Mode 7: Set Timer/Counter reset value

View: Set Value View

This mode allows adjusting of the reset value for the timer/counter. Adjust the
 value up or down as desired and touch the `A` key to save it; changes are lost
 if not saved.

**Important:** You must touch the `A` key to save the new value! If you do not,
 the new value will be lost upon exiting the set value view.

#### Mode 10: Set Options Bits

View: Set Bits View

This mode is where we configure a number of options that govern the clock's
 behavior. These are options that can be either on or off (enabled or disabled).
 As such, bits represent them nicely. Each LED has a label on the back of the
 PCB that describes what it indicates is enabled or disabled in this mode, but
 we'll document them here, as well:

* LED 1: Display BCD - if set, BCD is used in all displays, otherwise binary is
 used. This setting affects all displays in all modes and menus.
* LED 2: 12-hour mode - if set, the time is displayed as a 12-hour clock
 (AM/PM), otherwise it will display as a 24-hour clock.
* LED 3: Status LED as PM indicator - if set, the status LED will light in the
 current `1` color if it is PM or the `0` color if it is AM.
* LED 4: Hourly Chime - if set, at the top of every hour, the clock will
 indicate the current hour in binary audibly using a series of high (1) and low
 (0) beeps.
* LED 5: DST Enabled - if set, the clock will adjust itself automatically for
 daylight savings time (DST) based on values configured in settings 24 through
 29 (see below).
* LED 6: Display Fahrenheit - temperature is shown in degrees Fahrenheit if set,
 Celsius otherwise.
* LED 7: Fade Across Slots - if set, display colors will gradually change as
 time progresses from one configured time slot to the next. If disabled, colors
 will abruptly change when a time slot configured for color change is reached.
* LED 8: Automatic Intensity Adjustment - if set, the display brightness will
 automatically adjust based on the ambient light level. If clear, manual
 adjustment is enabled when the time, date, or temperature is displayed.
* LED 9: DMX-512 Extended mode - if set, additional DMX-512 channels are used
 enabling enhanced control of the clock via the control protocol.
* LED 10: Startup To Toggle - if set, the clock defaults to mode 2 when powered
 on or when left idle for more than one minute. Otherwise, mode 1 is the default.
* LED 11: MSbs Off - when set, LEDs that would otherwise be leading zeros are
 turned off in the time, date, temperature, and timer/counter views.

#### Modes 11 through 13: Time slot/Alarm behavior bits

View: Set Bits View

Each of these three modes allows enabling or disabling of each of the three
 behaviors described in the "Time Slots" section above. LEDs 1 through 8
 correspond to time slots 1 through 8, LED 9 corresponds to the timer/counter,
 and (on hardware version 4+) LEDs 10 and 11 correspond to the external alarm
 input pins (latching and momentary, respectively).

Mode 11 allows enabling or disabling the audible alarms/beeping, mode 12 allows
 enabling or disabling the display blinking, and mode 13 allows enabling or
 disabling color changes.

By default, color changes are enabled for all eight time slots while the audible
 alarms and display blinking are disabled for all eight time slots.

#### Modes 20 through 22: Time, Date, and Temperature display durations

View: Set Value View

As was described above, these three modes allow the adjustment of how long (in
 seconds) the time, date, and temperature will appear when the clock is
 operating in mode 2.

#### Mode 23: Fade duration

View: Set Value View

This mode allows the adjustment of the duration of the fading that occurs as
 bits on the display toggle from `0` to `1` or vice-versa. This effect is used
 in display modes 1, 2, and 3. The value set describes the fade duration in
 milliseconds and the default is 100.

#### Modes 24 through 29: Daylight Savings Time (DST) settings

View: Set Value View

These modes allow the configuration of exactly when clock adjustments occur due
 to DST beginning and ending. The default values correspond with DST changes
 that occur in the US.

Mode 24 allows setting the month in which DST begins.\
Mode 25 allows setting the day of week ordinal (first, second, third, fourth) in
 which DST begins.\
Mode 26 allows setting the month in which DST ends.\
Mode 27 allows setting the day of week ordinal (first, second, third, fourth) in
 which DST ends.\
Mode 28 allows setting the day of the week on which the DST time change occurs.\
Mode 29 allows setting the hour at which the DST time change occurs.

#### Mode 30: Minimum Intensity

View: Set Value View

This mode allows the adjustment of the minimum intensity allowed when automatic
 intensity adjustments are enabled (see mode 10). It is intended to keep the
 display from dimming _too much_.

#### Mode 31: Flicker Reduction

View: Set Value View

This is a technique used on pre-version 4 clocks that use the TLC5947 LED
 drivers. When the display intensity drops below the value determined by this
 setting, the hardware will strobe the drivers' BLANK pin as data is latched
 into the drivers, helping to mitigate flickering or pulsing of the display when
 frequent display refreshes are occurring. Combined with the setting defined in
 mode 36, this effect can be nearly eliminated. Note that this setting is not
 used by version 4+ clocks as it is not necessary with the TLC5951 LED drivers.

#### Mode 32: Current Drive

View: Set Value View

This mode allows software-based adjustment of the level of current the TLC59xx
 drivers allow through the display LEDs. In doing so, it effectively makes the
 LEDs dimmer or brighter. Lower current levels may lengthen the life of the LEDs
 and will also keep the drivers cooler.

#### Mode 33: Temperature Calibration

View: Set Value View

When using the STM32's internal temperature sensor, some calibration value is
 required as they're not all consistent. This mode allows the adjustment of this
 calibration value. Note that it is a **negative** value, so increasing the
 value will _decrease_ the temperature readout.

#### Mode 34: Beeper Volume

View: Set Value View

This mode allows adjusting of the beeper volume. Higher numbers mean a louder
 beeper. This setting affects all sounds the beeper produces. Easy enough, yeah?

#### Mode 35: Date Format

View: Set Value View

This mode allows adjusting of the way the year, month, and day are arranged on
 the display when the date appears. The possible values are zero (the default),
 one, or two, and they correspond to the following arrangements:
* 0: Year, Month, Day
* 1: Day, Month, Year
* 2: Month, Day, Year

#### Mode 36: Display Hardware Refresh Interval

View: Set Value View

This mode allows adjustment of the display hardware refresh interval. It
 determines the maximum frequency at which data may be latched into the TLC59xx
 drivers. On pre-version 4 hardware that uses the TLC5947 drivers, this setting,
 in conjunction with the setting defined in mode 31, helps reduce flicker or
 pulsing during frequent display refreshes. This setting should be left at the
 default value of zero on version 4+ clocks as the TLC5951 drivers have a
 better, built-in mechanism that eliminates flicker and pulsing. For pre-v4
 clocks, the recommended value for this setting is ten (10).

#### Mode 39: DMX-512 Address

View: Set Value View

This mode allows setting of the clock's DMX-512 address. If DMX-512 is not in
 use, this setting is irrelevant. See the DMX-512 parameters document for more
 information.

#### Modes 40 through 47: Set Slot Times

View: Set Time/Date View

These modes allow setting of the time for each of the eight time slots. See the
"Time Slots" section above for more information.

#### Modes 50 through 62: Set (Slot) Colors

View: Set Colors View

These modes allow setting of the color pairs associated with each of the eight
 time slots, the date, the temperature, the timer/counter, the main menu view,
 and the configuration/settings (Set Time/Date, Set Bits, and Set Value) views.
 See the "Time Slots" section above for more information.



 **Footnote:** Yes, I said **year**, **month**, **day**, the way the date is
  _supposed_ to be written/displayed. ;) If you don't like it, there is a setting
  that can be modified to change it. You're welcome. :p
