# Hardware Configuration

## Validated development setup

| Component | Current configuration | Status |
|---|---|---|
| Microcontroller | Wemos D1 Mini, ESP8266 | Tested |
| Display | 1.3-inch, 128×64 SH1106 monochrome OLED | Tested |
| Display interface | I²C | Tested |
| Display library | U8g2 | Tested |
| Touch input | TTP223 | Planned; not integrated |
| Motion input | ADXL345 | Planned; not integrated |
| Battery | 800 mAh LiPo | Planned; not integrated |
| Charger/protection | TP4056 USB-C module | Planned; not integrated |
| Enclosure | 3D-printed cube | Future work |

## OLED connections

| OLED pin | Wemos D1 Mini pin | ESP8266 GPIO |
|---|---|---|
| VCC | 3V3 | — |
| GND | GND | — |
| SDA | D2 | GPIO4 |
| SCL | D1 | GPIO5 |

Firmware display constructor: `U8G2_SH1106_128X64_NONAME_F_HW_I2C`.

The OLED has been confirmed working with the current firmware on the Wemos D1 Mini.

## Planned sensor bus

The ADXL345 is an I²C device and is intended to share the I²C bus with the OLED. Confirm its address selection, voltage compatibility, and wiring on the actual breakout board before integration. The TTP223 is a digital touch input and will require a suitable GPIO and defined idle/active logic.

## Power design — not yet approved

The TP4056 provides Li-ion charging and, on protected variants, cell protection. It does **not** regulate the cell voltage to 3.3 V. A single-cell LiPo voltage varies across its discharge range, so it must not be connected directly to the Wemos 3V3 pin.

Before battery integration:
1. Identify the exact TP4056 module variant and protection arrangement.
2. Select a regulator suitable for the Wemos input/current requirements.
3. Define the charging, load-sharing, and power-path arrangement.
4. Verify polarity, voltage, current limits, and thermal behaviour.
5. Measure current consumption in awake, sleeping, and deep-sleep states.

Do not treat the planned battery parts as an implemented or validated power system.

## Mechanical design dependency

Design the enclosure only after measuring the actual Wemos board, OLED PCB, sensor boards, battery, charging port, and mounting clearances.
