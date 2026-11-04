# Cubo — Interactive OLED Desk Companion

> A compact desktop companion with animated expressions, capacitive-touch interaction, and motion-aware reactions.

**Project status:** Firmware prototype operational; power system and enclosure are still in development.

Cubo is an embedded-electronics project that combines a small OLED display, custom animation firmware, touch input, and an accelerometer to create a responsive desk companion. The current prototype runs on a **Wemos D1 mini (ESP8266)** and uses a **1.3-inch 128×64 SH1106 OLED**.

The repository is being developed incrementally. Hardware features are marked as tested only where they have been brought up and verified on the current prototype; planned product features are kept separate from the working baseline.

---

## Contents

- [Current prototype](#current-prototype)
- [Features](#features)
- [Hardware](#hardware)
- [Wiring](#wiring)
- [Firmware](#firmware)
- [Repository layout](#repository-layout)
- [Getting started](#getting-started)
- [Development status](#development-status)
- [Roadmap](#roadmap)
- [Contributing and project notes](#contributing-and-project-notes)
- [License](#license)

---

## Current prototype

| Subsystem | Current implementation | Status |
|---|---|---|
| Microcontroller | Wemos D1 mini (ESP8266) | Tested |
| Display | 1.3-inch, 128×64 SH1106 OLED | Tested |
| Touch input | TTP223 capacitive touch module | Tested |
| Motion input | ADXL345 3-axis accelerometer | Tested |
| Firmware | Arduino framework, modular C++ | Integrated prototype |
| Battery and charging | LiPo cell with TP4056 charging module | Not yet integrated/validated |
| Enclosure | Custom 3D-printed enclosure | Pending |

**Important:** The current working electronics are a prototype, not yet a finalized battery-powered product. The TP4056 and LiPo arrangement, voltage regulation, charging behaviour, and runtime still require engineering validation. Do not connect a LiPo cell directly to the Wemos 3V3 pin.

## Features

### Display and expressions

- SH1106 OLED rendering through the U8g2 graphics library.
- Bitmap-based facial expressions and non-blocking animation sequences.
- Happy, blink, wink, drowsy, sleeping, waking, knocked-out, and recovery expressions.

### Touch interaction

- TTP223 active-HIGH touch input.
- Debounced touch-event handling.
- Touch while awake triggers a left wink.
- Touch while sleeping or drowsy starts the wake animation.
- Touches during the wake sequence are ignored so the animation is not restarted.

### Motion interaction

- ADXL345 accessed directly over I²C using Arduino's `Wire.h`; no separate ADXL345 library is required.
- Startup calibration records the prototype's natural resting orientation.
- Sustained orientation displacement triggers the Knocked Out expression.
- Returning to the calibrated orientation triggers Recovery 1 → Recovery 2 → Happy.
- Motion detection has priority over ordinary touch and animation behaviour.

### Idle behaviour

- Random blink interval: 3–6 seconds.
- After 15 seconds without interaction, Cubo enters its drowsy/sleep sequence.
- While sleeping, it automatically begins the wake sequence after 25 seconds.
- These are firmware behaviour timers, not low-power or deep-sleep modes.

---

## Hardware

### Bill of materials — current prototype

| Component | Role | Status |
|---|---|---|
| Wemos D1 mini (ESP8266) | Main controller | Tested |
| 1.3-inch 128×64 SH1106 OLED | Face display | Tested |
| TTP223 capacitive touch module | Touch interaction | Tested |
| ADXL345 accelerometer | Orientation/displacement detection | Tested |
| LiPo battery | Portable power | Planned; not validated |
| TP4056 charging/protection module | LiPo charging | Planned; not validated |
| 3.3 V power regulation / power-path solution | Stable system supply | To be finalized |
| 3D-printed enclosure | Mechanical housing | Pending |

### Pin connections

The OLED and ADXL345 share the I²C bus.

| Signal | Wemos D1 mini pin | ESP8266 GPIO | Connected to |
|---|---|---:|---|
| I²C SDA | D2 | GPIO4 | OLED SDA and ADXL345 SDA |
| I²C SCL | D1 | GPIO5 | OLED SCL and ADXL345 SCL |
| Touch signal | D5 | GPIO14 | TTP223 OUT/SIG |
| 3.3 V | 3V3 | — | OLED, ADXL345, TTP223 VCC |
| Ground | GND | — | Common ground |

**I²C addresses observed on the prototype:**

- SH1106 OLED module: `0x3C`
- ADXL345 (SDO low): `0x53`

For the ADXL345, configure the module for I²C operation (CS high) and use SDO low for address `0x53`. Confirm the pin labels on your particular breakout before wiring.

---

## Firmware

The current firmware is maintained as a flat Arduino sketch directory:

`firmware/CuboBot_Firmware/`

| File | Responsibility |
|---|---|
| `CuboBot_Firmware.ino` | Application entry point and main loop |
| `Config.h` | Pins, timing, and sensor thresholds |
| `Display.h / Display.cpp` | OLED initialization and rendering |
| `Expressions.h / Expressions.cpp` | Expression identifiers and bitmap assets |
| `AnimationController.h / .cpp` | Timed, non-blocking animation playback |
| `BehaviourFSM.h / .cpp` | Behaviour state machine and transitions |
| `InputManager.h / .cpp` | TTP223, ADXL345, and serial test input |
| `README.md` | Firmware wiring, behaviour, and test notes |

The code is organized into separate display, animation, behaviour, and input modules while keeping all sketch files together for straightforward use in the Arduino IDE.

### Behaviour priority

The behaviour controller gives confirmed physical displacement the highest priority:

1. **Knocked out:** hold the knockout expression while the sensor remains displaced.
2. **Recovering:** after the natural orientation is confirmed, play Recovery 1 and Recovery 2, then return to Happy.
3. **Waking / sleeping / awake:** normal touch, idle, blink, and wake behaviour runs when not overridden by motion states.

### ADXL345 calibration

On startup, keep Cubo still in its intended natural resting orientation while the firmware collects calibration samples. The stored reference is recalculated at every reset; it is not persisted to non-volatile memory. Mounting orientation and thresholds should be rechecked when the sensor is moved to the final PCB or enclosure.

If the ADXL345 is not detected or calibration fails, the firmware reports the condition over Serial and continues with OLED, touch, and serial behaviour; automatic displacement detection is disabled.

---

## Getting started

### Requirements

- Wemos D1 mini (ESP8266)
- SH1106 128×64 OLED, TTP223 module, and ADXL345 breakout
- Arduino IDE
- ESP8266 board support package installed in Arduino IDE
- U8g2 library
- USB data cable

### Upload

1. Clone or download this repository.
2. Open `firmware/CuboBot_Firmware/CuboBot_Firmware.ino` in Arduino IDE.
3. Install the **U8g2** library through the Arduino IDE Library Manager.
4. Select the Wemos D1 mini / appropriate ESP8266 board and its connected port.
5. Verify the wiring and power the modules from the board's 3.3 V rail.
6. Compile and upload the sketch.
7. Open Serial Monitor at **115200 baud**.
8. Reset the board while holding Cubo still in its intended natural orientation until calibration completes.

See [the firmware README](firmware/CuboBot_Firmware/README.md) for sensor setup, startup messages, and serial test commands.

---

## Repository layout

```text
Cubo_Bot/
├── README.md
├── LICENSE
├── firmware/
│   ├── CuboBot_Firmware/       # Current integrated firmware
│   └── CuboBot_Firmware_Setup/ # Earlier setup and expression-test snapshots
├── hardware/                   # Hardware design and wiring resources
├── docs/                       # Project documentation
├── tests/                      # Test resources
├── media/                      # Project media
└── Photopea/                   # Expression artwork working files (if present)
```

Some directories are works in progress and may be empty or contain local assets as the project evolves.

---

## Development status

### Completed and tested on the current prototype

- [x] Wemos D1 mini and SH1106 OLED bring-up
- [x] OLED bitmap rendering and expression set
- [x] TTP223 touch detection and debounce
- [x] ADXL345 I²C communication and startup calibration
- [x] Knocked Out and Recovery behaviour
- [x] Awake, blink, wink, sleep, and wake sequences
- [x] Integration of display, animation, input, and behaviour modules

### In progress / not yet validated

- [ ] Calibrate motion thresholds with the sensor in its final mechanical mounting
- [ ] Finalize LiPo charging, protection, voltage regulation, and power-path design
- [ ] Measure active and idle current consumption
- [ ] Validate battery runtime and charging safety
- [ ] Design and print the enclosure
- [ ] Integrate the electronics into the enclosure and complete end-to-end testing

---

## Roadmap

| Phase | Focus | Intended outcome |
|---|---|---|
| 1 | Firmware and sensor validation | Stable display, touch, and motion behaviour |
| 2 | Motion calibration | Reliable orientation detection in final mounting |
| 3 | Power architecture | Safe, regulated, rechargeable operation |
| 4 | Power optimization | Measured current draw and practical runtime |
| 5 | Mechanical design | Enclosure, fit checks, and touch-surface validation |
| 6 | Final integration | Assembled, tested desk companion |

---

## Contributing and project notes

Cubo is developed incrementally. Keep changes modular, document hardware revisions and pin changes, and validate each subsystem before integrating additional complexity.

When reporting an issue, include the board variant, wiring, Arduino IDE/core version, library versions, Serial Monitor output, and the behaviour observed.

## License

This project is licensed under the [MIT License](LICENSE).
