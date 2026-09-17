# Arduino Due Projects



A collection of Arduino Due experiments covering displays, USB HID, DAC/audio, timers, RTC/RTT, low-power modes, and direct SAM3X8E register programming.

Each folder is an independent project. The collection is mainly about experimenting with the Due and learning what is happening underneath the normal Arduino API.

> \*\*Note:\*\* The unfinished Dino/OLED game project is not included here or in the published project list.

\---

## Projects

|Project|Description|
|-|-|
|[`Changing\_Atsam3X8E\_Clock\_speed\_with\_PLL`](#changing-the-atsam3x8e-clock-speed-with-pll)|Changes the SAM3X8E clock using PLLA and the master-clock prescaler.|
|[`Controlling\_PC\_Mouse\_with\_analog\_joystick`](#controlling-a-pc-mouse-with-an-analog-joystick)|Uses a joystick connected to the Due to control a PC mouse over USB.|
|[`DAC\_Test`](#dac-test)|Sends 12-bit values to the Due's DAC and produces an analog output.|
|[`Drawing\_Large\_480x320\_on\_TFT\_and\_animation`](#480x320-tft-image-and-animation)|Displays a large RGB565 image on a TFT and runs a simple animation.|
|[`Electret\_Microphone\_Playback`](#electret-microphone-playback)|Plays stored microphone samples through DAC1.|
|[`Electret\_Microphone\_Record`](#electret-microphone-record)|Samples an electret microphone and outputs the raw samples over Serial.|
|[`Inbuilt\_RTC\_Demo`](#built-in-rtc-demo)|Configures and reads the SAM3X8E's internal real-time clock.|
|[`IR\_Receiver`](#ir-receiver)|Decodes NEC-style infrared remote-control signals.|
|[`Keyboard\_Simulator\_Test`](#keyboard-simulator-test)|Makes the Due appear to a computer as a USB keyboard.|
|[`Mouse\_Simulator\_Test`](#mouse-simulator-test)|Makes the Due appear as a USB mouse and sends movement commands.|
|[`Multiplexing\_4\_7\_segment\_displays`](#multiplexing-four-7-segment-displays)|Drives four 7-segment digits using multiplexing.|
|[`Native\_TX\_RX\_LED\_Control`](#native-txrx-led-control)|Directly controls the Due's Native USB TX/RX LEDs.|
|[`Reading\_internal\_core\_temp\_of\_CPU`](#reading-the-internal-cpu-temperature)|Reads the SAM3X8E's internal temperature-sensor ADC channel.|
|[`Reading\_Ultrasonic\_distance\_sensor\_with\_timer\_capture\_mode`](#ultrasonic-distance-with-timer-capture)|Measures an ultrasonic echo pulse using a hardware timer capture channel.|
|[`Remote\_Controlled\_Mouse\_Native\_Port\_and\_IR`](#ir-remote-controlled-mouse-and-keyboard)|Combines an IR remote with native USB mouse and keyboard controls.|
|[`RTC\_Clock\_on\_4\_7\_segment\_display\_multiplex`](#rtc-clock-on-a-four-digit-display)|Displays RTC hours and minutes on a multiplexed 4-digit display.|
|[`RTT\_Demo`](#rtt-demo)|Uses the SAM3X8E Real-time Timer Timer and its alarm interrupt.|
|[`Sleep\_...backup...\_mode\_demo`](#backup-mode-demo)|Demonstrates backup mode and waking the Due using the RTT.|
|[`Sleep\_...sleep...\_mode\_demo`](#sleep-mode-demo)|Demonstrates normal CPU sleep and waking from an interrupt.|
|[`Sleep\_...wait...\_mode\_demo`](#wait-mode-demo)|Demonstrates SAM3X8E wait mode with an RTT wake-up.|
|[`Software\_Reset\_Demo`](#software-reset-and-reset-reason)|Reads the SAM3X8E reset controller to determine why the MCU reset.|
|[`SSD\_1306\_128x32\_OLED\_Display\_Demo`](#ssd1306-128x32-oled-demo)|Demonstrates text, shapes, bitmaps, scrolling, and other SSD1306 features.|
|[`Timer\_capture\_mode\_demo`](#timer-capture-mode)|Measures pulse widths using the SAM3X8E timer capture hardware.|
|[`Timer\_waveform\_mode\_demo`](#timer-waveform-mode)|Generates a precise waveform using a hardware timer.|
|[`TRNG\_Demo`](#true-random-number-generator)|Reads random data from the SAM3X8E's built-in hardware TRNG.|

\---

## Hardware

All projects target the **Arduino Due / ATSAM3X8E**.

The Due has a 32-bit ARM Cortex-M3, 84 MHz CPU, 3.3 V logic, 12-bit ADC, real DAC outputs, hardware timers, RTC/RTT, a hardware TRNG, and native USB.

Several sketches access SAM3X8E registers directly, so they are not drop-in examples for Uno/Nano boards.

**Important:** the SAM3X8E is a 3.3 V MCU. Check the voltage requirements of external hardware before connecting it.

\---

## Getting started

Install the Arduino IDE and Arduino SAM board support, then select:

```text
Board: Arduino Due
Processor: appropriate SAM3X8E option
```

Each project can be opened and uploaded independently.

### USB

The Due has a Programming Port and a **Native USB Port**. Use the Native USB port for:

* `Keyboard\_Simulator\_Test`
* `Mouse\_Simulator\_Test`
* `Controlling\_PC\_Mouse\_with\_analog\_joystick`
* `Remote\_Controlled\_Mouse\_Native\_Port\_and\_IR`

### Libraries

Most sketches only need the Due board package. The display projects additionally use:

* **Adafruit GFX Library**
* **Adafruit SSD1306**
* **MCUFRIEND\_kbv** for the TFT project

`Keyboard` and `Mouse` are provided by the Due core.

\---

# Project Details

## Changing the ATSAM3X8E clock speed with PLL

**Folder:** `Changing\_Atsam3X8E\_Clock\_speed\_with\_PLL`

Directly changes the SAM3X8E clock tree using the main clock, PLLA, the master-clock prescaler, PMC registers, `SystemCoreClock`, and `SysTick`.

The sketch changes the clock before initializing Serial so the serial baud rate is based on the new clock. It is a useful example of moving from Arduino-level code into direct MCU configuration.

**Note:** changing the system clock can affect peripherals that depend on it.

## Controlling a PC mouse with an analog joystick

**Folder:** `Controlling\_PC\_Mouse\_with\_analog\_joystick`

Turns a joystick into a USB mouse.

|Joystick|Due|
|-|-|
|X|`A0`|
|Y|`A1`|

The ADC is set to 8-bit resolution and the joystick values are mapped to mouse movement. The joystick press is also used for clicking.

Use the **Native USB port**.

## DAC test

**Folder:** `DAC\_Test`

A simple experiment with the Due's real DAC output. It uses 12-bit resolution, so values range from `0` to `4095`, and values can be entered through Serial.

```text
0       minimum
2048    roughly half-scale
4095    maximum
```

This is a useful comparison with the PWM-style `analogWrite()` commonly seen on AVR Arduino boards.

## 480×320 TFT image and animation

**Folder:** `Drawing\_Large\_480x320\_on\_TFT\_and\_animation`

Uses `Adafruit\_GFX` and `MCUFRIEND\_kbv` to display a 480×320 RGB565 image, wait, run a vertical-line animation, and redraw the image.

A 480×320 RGB565 image is about **307 KB**, so this sketch is much larger than the other demos.

## Electret microphone record

**Folder:** `Electret\_Microphone\_Record`

Samples an electret microphone on `A3` using the Due's 12-bit ADC and prints the raw samples over Serial at **460800 baud**.

The intended workflow is:

```text
microphone → A3 → ADC → Serial → samples\[] in playback sketch
```

Large recordings can be split into multiple chunks to avoid losing Serial Monitor output.

## Electret microphone playback

**Folder:** `Electret\_Microphone\_Playback`

Plays stored ADC samples from a `samples\[]` array through **DAC1**. It is intended to be used with the recording project.

The recording and playback sample timing need to match, and the DAC should feed an appropriate audio amplifier/circuit rather than a speaker directly.

## Built-in RTC demo

**Folder:** `Inbuilt\_RTC\_Demo`

Configures and reads the SAM3X8E's built-in RTC directly. It writes date/time registers and prints them once per second.

The RTC fields use **BCD**, making this a useful example of peripheral registers, masks/shifts, and BCD conversion.

## IR receiver

**Folder:** `IR\_Receiver`

Decodes NEC-style IR signals on **pin 13** without using an IR library. It measures HIGH/LOW pulse lengths with `micros()`, reconstructs the 32-bit command, and handles the NEC repeat code.

It is a nice example of decoding a protocol directly from GPIO timing.

## Keyboard simulator test

**Folder:** `Keyboard\_Simulator\_Test`

Uses the Due's Native USB interface as a keyboard. After startup, it sends keyboard commands to open Windows Run, launch Notepad, and type text.

Use the **Native USB port**.

## Mouse simulator test

**Folder:** `Mouse\_Simulator\_Test`

A simple native USB HID test that repeatedly sends random mouse movements.

Use the **Native USB port**.

## Multiplexing four 7-segment displays

**Folder:** `Multiplexing\_4\_7\_segment\_displays`

Drives four digits by rapidly switching between them.

**Segments:** A=`51`, B=`53`, C=`35`, D=`37`, E=`39`, F=`49`, G=`47`  
**Digit select:** 1=`29`, 2=`27`, 3=`25`, 4=`23`

Numbers from `0–9999` can be entered through Serial at **115200 baud**. The short Serial timeout helps prevent flicker by keeping the display refresh running.

## Native TX/RX LED control

**Folder:** `Native\_TX\_RX\_LED\_Control`

Directly controls the Due's Native USB LEDs:

```text
72 → RX LED
73 → TX LED
```

They are active-low (`LOW = ON`, `HIGH = OFF`) and the example alternates them every 500 ms.

## Reading the internal CPU temperature

**Folder:** `Reading\_internal\_core\_temp\_of\_CPU`

Reads the SAM3X8E internal temperature sensor through **ADC channel 15** and converts the result to an approximate Celsius value.

This is a chip-temperature estimate, not a calibrated thermometer.

## Ultrasonic distance with timer capture

**Folder:** `Reading\_Ultrasonic\_distance\_sensor\_with\_timer\_capture\_mode`

Measures an ultrasonic sensor's echo pulse using hardware timer capture.

```text
Trigger → pin 3
Echo    → pin 2
```

The timer captures the rising edge in `RA` and falling edge in `RB`; their difference gives the pulse width, which is converted to distance.

Unlike `pulseIn()`, the timer hardware records the edge timestamps without the CPU constantly polling the input.

## IR remote controlled mouse and keyboard

**Folder:** `Remote\_Controlled\_Mouse\_Native\_Port\_and\_IR`

Combines NEC IR decoding with native USB mouse/keyboard HID.

|IR code|Action|
|-|-|
|`0xFFE817`|Right click|
|`0xFF48B7`|Move up|
|`0xFF02FD`|Move left|
|`0xFF32CD`|Left click|
|`0xFF20DF`|Move right|
|`0xFF7887`|Move down|
|`0xFF28D7`|Scroll down|
|`0xFFF00F`|Scroll up|
|`0xFF609F`|Toggle movement speed|

The IR receiver is on pin 13. The sketch also sends `Ctrl + Windows + O` to open the Windows on-screen keyboard.

Use the **Native USB port**.

## RTC clock on a four-digit display

**Folder:** `RTC\_Clock\_on\_4\_7\_segment\_display\_multiplex`

Combines the RTC with the multiplexed 4-digit display to show:

```text
HH:MM
```

The colon uses pin `52`, and the display uses the same segment/digit pins as the standalone multiplexing project.

The starting time is set with BCD values such as `setRTC(0x19, 0x18)`. Accuracy depends on the slow-clock source, so this is mainly a learning/demo project rather than precision timekeeping.

## RTT demo

**Folder:** `RTT\_Demo`

Uses the SAM3X8E **Real-time Timer (RTT)** as a counter and alarm. It enables the RTT interrupt, schedules an alarm, and lets another alarm be set through Serial.

Important registers include `RTT\_MR`, `RTT\_VR`, `RTT\_AR`, and `RTT\_SR`.

The interrupt handler sets a flag while `loop()` handles Serial output — a useful embedded-programming pattern.

# Low-power mode demos

There are three related projects for exploring the SAM3X8E's low-power modes:

* `Sleep\_...sleep...\_mode\_demo`
* `Sleep\_...wait...\_mode\_demo`
* `Sleep\_...backup...\_mode\_demo`

They're useful to look at side-by-side because each one goes a little deeper into power management.

\---

## Sleep mode demo

**Folder:** `Sleep\_...sleep...\_mode\_demo`

Puts the CPU into normal sleep and uses the RTT as a timed wake-up source. The basic flow is:

```text
configure RTT → set alarm → sleep → interrupt → wake
```

SysTick is disabled before sleeping so it doesn't immediately wake the CPU.

## Wait mode demo

**Folder:** `Sleep\_...wait...\_mode\_demo`

Demonstrates the SAM3X8E's deeper wait mode using the Power Management Controller and ARM sleep settings, with the RTT providing the wake-up event.

## Backup mode demo

**Folder:** `Sleep\_...backup...\_mode\_demo`

Demonstrates the deepest low-power state in this collection. The main CPU/core power domain is turned off, so waking does not simply continue after `\_\_WFE()`; the device goes through reset/startup.

The RTT is configured as the wake-up source.

The important concept is the difference between **stopping the CPU** and **removing power from the CPU/core**.

## Software reset and reset reason

**Folder:** `Software\_Reset\_Demo`

Reads the SAM3X8E Reset Controller to determine why the MCU reset, including sources such as power-on, backup, watchdog, software, and reset-pin events.

Useful when debugging firmware that unexpectedly restarts.

## SSD1306 128×32 OLED demo

**Folder:** `SSD\_1306\_128x32\_OLED\_Display\_Demo`

A general graphics demo using `Wire`, Adafruit GFX, and Adafruit SSD1306 with a 128×32 I2C display at `0x3C`.

It covers text, shapes, bitmaps, scrolling, inversion, animation, and contrast changes. It's essentially a collection of small SSD1306/GFX examples in one sketch.

## Timer capture mode

**Folder:** `Timer\_capture\_mode\_demo`

Configures `TC0 / Channel 0` in capture mode. Rising and falling edges are timestamped in `TC\_RA` and `TC\_RB`, allowing pulse width to be calculated in hardware.

The sketch also demonstrates SAM3X8E PIO peripheral multiplexing, where a physical pin is assigned to the timer instead of ordinary GPIO.

Key registers include `TC\_CCR`, `TC\_CMR`, `TC\_RC`, `TC\_RA`, `TC\_RB`, `TC\_CV`, and `TC\_SR`.

## Timer waveform mode

**Folder:** `Timer\_waveform\_mode\_demo`

Uses `TC0 / Channel 0` to generate a hardware waveform. The example uses a roughly 42 MHz timer clock with values such as `RC = 42000` and `RA = 21000`, producing about 1 kHz at roughly 50% duty cycle.

The output is routed to pin 2 through the peripheral multiplexer, so the timer handles the timing without software toggling the pin.

## True random number generator

**Folder:** `TRNG\_Demo`

Enables the SAM3X8E hardware TRNG, waits for data to become ready, and reads `TRNG->TRNG\_ODATA`.

The main registers are:

```text
TRNG\_CR
TRNG\_ISR
TRNG\_ODATA
```

A small warm-up period is included before using the generated values.

# Hardware / pin quick reference

|Pin|Main use|
|-:|-|
|`2`|Timer peripheral / ultrasonic echo|
|`3`|Ultrasonic trigger|
|`13`|IR receiver or built-in LED|
|`A0`|Joystick X|
|`A1`|Joystick Y|
|`A3`|Electret microphone|
|`23, 25, 27, 29`|7-segment digit select|
|`35, 37, 39, 47, 49, 51, 53`|7-segment segments|
|`52`|RTC display colon|
|`72`|Native USB RX LED|
|`73`|Native USB TX LED|
|`DAC0`|DAC test output|
|`DAC1`|Microphone playback|

Pins can overlap between projects because each sketch is independent.

# Arduino API vs direct SAM3X8E hardware

Some projects use normal Arduino functions:

```cpp
digitalWrite();
analogRead();
analogWrite();
Mouse.move();
Keyboard.print();
```

Others configure SAM3X8E peripherals directly:

```cpp
PMC->...
RTC->...
RTT->...
TC0->...
ADC->...
TRNG->...
PIOB->...
```

The register-level sketches are more verbose, but they expose things like timer channels, clock sources, capture edges, waveform modes, and peripheral pin multiplexing.

# Capture vs waveform mode

The timer projects show two sides of the same hardware.

**Capture mode** measures an external signal:

```text
signal → timer → RA/RB → pulse width
```

**Waveform mode** generates a signal:

```text
timer clock → counter → comparisons → output pin
```

The ultrasonic project uses capture mode, while the waveform demo generates its own output.

# RTC vs RTT

The **RTC** is for calendar-style time:

```text
seconds / minutes / hours / date
```

The **RTT** is more of a counter plus alarm, making it useful for timed events, wake-ups, and low-power experiments.

# Troubleshooting

### USB keyboard/mouse doesn't work

Use the **Native USB port** and give the computer a moment to enumerate the device after reset.

### OLED is blank

Check SDA/SCL, power, the `0x3C` address, display size, and the Adafruit GFX/SSD1306 libraries.

### TFT doesn't display correctly

`MCUFRIEND\_kbv` detects the controller, but visually similar shields can use different controller chips.

### 7-segment display flickers

The display is multiplexed and must be refreshed continuously. Avoid long delays in `loop()` and verify the segment/digit wiring.

### RTC drifts

The RTC examples are for learning. Accuracy depends on the slow-clock source and is not equivalent to a dedicated precision RTC.

### Audio sounds wrong

The recording and playback projects rely on matching sample timing. DAC output should normally feed an appropriate audio circuit.

### Timer behaves unexpectedly

Check the timer channel, clock divider, capture edge, RA/RB/RC values, peripheral pin selection, and physical pin. These sketches directly configure the SAM3X8E hardware.

# Suggested learning path

If you're learning the Due, a useful order is:

1. **DAC Test**
2. **Native TX/RX LED Control**
3. **Mouse / Keyboard Simulator**
4. **SSD1306 OLED Demo**
5. **4-digit 7-segment display**
6. **Built-in RTC**
7. **TRNG / internal temperature**
8. **Timer waveform and capture**
9. **Ultrasonic timer capture**
10. **RTT**
11. **Sleep / Wait / Backup**
12. **Clock / PLL**

They're independent, so jumping around is completely fine.

# Notes before using the projects

* These sketches target the **Arduino Due / SAM3X8E**.
* Several projects use direct register access and are not portable to other Arduino boards.
* External hardware and wiring vary by project.
* The TFT image and microphone samples use a significant amount of program memory.
* Native USB is required for the HID projects.
* The temperature example is an estimate, not a calibrated thermometer.
* The RTC demos are learning projects rather than precision timekeeping solutions.
* The low-power sketches intentionally stop clocks/CPU power, so they behave differently from normal Arduino code.
* Timer examples can take pins away from normal GPIO through the SAM3X8E peripheral multiplexer.

## Folder structure

One Arduino sketch per folder:

```text
DUE/
├── Changing\_Atsam3X8E\_Clock\_speed\_with\_PLL/
├── Controlling\_PC\_Mouse\_with\_analog\_joystick/
├── DAC\_Test/
├── Drawing\_Large\_480x320\_on\_TFT\_and\_animation/
├── Electret\_Microphone\_Playback/
├── Electret\_Microphone\_Record/
├── Inbuilt\_RTC\_Demo/
├── IR\_Receiver/
├── Keyboard\_Simulator\_Test/
├── Mouse\_Simulator\_Test/
├── Multiplexing\_4\_7\_segment\_displays/
├── Native\_TX\_RX\_LED\_Control/
├── Reading\_internal\_core\_temp\_of\_CPU/
├── Reading\_Ultrasonic\_distance\_sensor\_with\_timer\_capture\_mode/
├── Remote\_Controlled\_Mouse\_Native\_Port\_and\_IR/
├── RTC\_Clock\_on\_4\_7\_segment\_display\_multiplex/
├── RTT\_Demo/
├── Sleep\_...backup...\_mode\_demo/
├── Sleep\_...sleep...\_mode\_demo/
├── Sleep\_...wait...\_mode\_demo/
├── Software\_Reset\_Demo/
├── SSD\_1306\_128x32\_OLED\_Display\_Demo/
├── Timer\_capture\_mode\_demo/
├── Timer\_waveform\_mode\_demo/
└── TRNG\_Demo/
```

## License

No license file or explicit license terms are included with the projects. Add a license here if you plan to publish the repository publicly.

\---

These are mostly hands-on experiments rather than production-ready libraries, so some sketches are intentionally simple or hardware-specific. If you want to dig deeper into the Arduino Due and the SAM3X8E, there is plenty here to experiment with.

