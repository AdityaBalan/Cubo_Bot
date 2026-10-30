# Cubo Bot

Cubo is a compact OLED desk companion with animated facial expressions and an event-driven behaviour model.

> **Current validated build:** Wemos D1 Mini (ESP8266) + 1.3-inch 128×64 SH1106 I²C OLED. The firmware foundation has been uploaded and tested on this hardware by the project developer.

## Current status

| Subsystem | Status | Notes |
|---|---|---|
| Wemos D1 Mini (ESP8266) | Tested | Current firmware target |
| 1.3-inch SH1106 OLED | Tested | 128×64, I²C, U8g2 |
| Expression bitmaps | Implemented | Nine monochrome frames stored in flash |
| Animation controller | Implemented | Non-blocking, `millis()`-based sequencing |
| Behaviour state machine | Implemented | Awake, drowsy, sleeping, waking |
| Serial input adapter | Implemented | Temporary input simulation |
| TTP223 touch sensor | Not integrated | Planned input source |
| ADXL345 accelerometer | Not integrated | Planned gesture input |
| LiPo + TP4056 + regulator | Not integrated | Power architecture still to be engineered |
| Enclosure | Not designed | Mechanical design follows hardware integration |

## Behaviour foundation

- Happy expression on startup.
- Automatic blink at a randomized 3–6 second interval.
- Drowsy transition after approximately 12 seconds without interaction.
- Sleep animation loops through two frames.
- Touch event wakes the device.
- Serial commands simulate inputs during development: `t` (touch), `l` (left tilt), `r` (right tilt).

## Repository layout

```text
firmware/
  CuboBot_Firmware/
    CuboBot_Firmware.ino
    Config.h
    Display/
    Expressions/
    Animation/
    Behaviour/
    Input/
docs/
  architecture.md
  hardware.md
  development-log.md
```

## Getting started

1. Install Arduino IDE and the **U8g2** library by oliver.
2. Open `firmware/CuboBot_Firmware/CuboBot_Firmware.ino`.
3. Select **LOLIN(WEMOS) D1 R2 & mini** (or the matching Wemos D1 Mini ESP8266 board package entry).
4. Upload to the Wemos D1 Mini.
5. Open Serial Monitor at **115200 baud** and send `t`, `l`, or `r`.

See [firmware setup](firmware/CuboBot_Firmware/README.md), [architecture](docs/architecture.md), [hardware notes](docs/hardware.md), and the [development log](docs/development-log.md).

## Development workflow

Work is staged on `develop`. Feature branches are reviewed through pull requests; `main` is reserved for stable milestones. Commits use conventional commit-style messages.

## Roadmap

1. Integrate and validate TTP223 touch input.
2. Integrate ADXL345 and tune gesture recognition.
3. Finalize regulated battery/power design and verify safe charging.
4. Design and validate the enclosure against measured hardware.
5. Implement low-power behaviour and perform runtime/reliability tests.

## Hardware safety note

The TP4056 is a Li-ion charger/protection module, not a 3.3 V regulator. Do not connect the LiPo directly to the Wemos 3V3 rail. Final battery integration requires a suitable regulated supply and a verified power-path design.

