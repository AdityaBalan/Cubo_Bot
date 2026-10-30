# Cubo Bot Firmware Foundation

**Target:** Wemos D1 Mini (ESP8266).  
**Display:** 1.3-inch 128×64 SH1106 I²C OLED.  
**Display library:** U8g2 by oliver.

## Modules

- `Display/`: SH1106 rendering.
- `Expressions/`: nine bitmap assets and expression registry.
- `Animation/`: non-blocking animation sequencer.
- `Behaviour/`: awake/drowsy/sleeping/waking finite-state machine.
- `Input/`: normalized input events; Serial is the temporary test adapter.
- `Config.h`: centralized timing constants.

## Arduino IDE setup

1. Install the ESP8266 board package and U8g2 library.
2. Open `CuboBot_Firmware.ino` from this directory.
3. Select the Wemos D1 Mini / LOLIN(WEMOS) D1 mini ESP8266 board entry.
4. Upload and open Serial Monitor at 115200 baud.

## OLED wiring

| OLED | Wemos D1 Mini |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | D2 / GPIO4 |
| SCL | D1 / GPIO5 |

The display driver uses `U8G2_SH1106_128X64_NONAME_F_HW_I2C`.

## Serial input simulation

| Character | Simulated event |
|---|---|
| `t` | Touch; wink while awake, wake while drowsy/sleeping |
| `l` | Left tilt; left wink while awake |
| `r` | Right tilt; right wink while awake |

## Behaviour

- Starts with Happy.
- Automatic blink every randomized 3–6 seconds.
- Drowsy transition after 12 seconds of inactivity.
- Sleep 1 is the transition; Sleep 2 and Sleep 3 form the sleep loop.
- Touch during drowsy/sleeping plays Wake and returns to Happy.

Animation timing uses `millis()`; the animation loop does not use blocking delays.

## Notes

The physical touch sensor and accelerometer are not integrated in this firmware milestone. Serial is a test adapter, not a claim of completed sensor integration. Battery operation and deep sleep are also outside this milestone.
