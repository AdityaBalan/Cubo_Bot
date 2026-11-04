# Cubo Firmware — Hardware and Behaviour Notes

This directory contains the current integrated Arduino firmware for the Cubo prototype.

## Hardware baseline

- **Controller:** Wemos D1 mini (ESP8266)
- **Display:** 1.3-inch, 128×64 SH1106 OLED using U8g2
- **Touch:** TTP223 capacitive sensor, active-HIGH output
- **Motion:** ADXL345 accelerometer, accessed directly over I²C using `Wire.h`

### Wiring

| Signal | Wemos D1 mini | Connected device |
|---|---|---|
| SDA | D2 / GPIO4 | OLED SDA and ADXL345 SDA |
| SCL | D1 / GPIO5 | OLED SCL and ADXL345 SCL |
| Touch input | D5 / GPIO14 | TTP223 OUT/SIG |
| 3.3 V | 3V3 | OLED, ADXL345, TTP223 VCC |
| Ground | GND | Common ground |

The OLED and ADXL345 share the I²C bus. Addresses observed on the prototype are OLED `0x3C` and ADXL345 `0x53` (SDO low). For ADXL345 I²C mode, CS must be high.

## Implemented behaviour

### Display and animation

- SH1106 rendering with the U8g2 library.
- Bitmap-based expressions and timed animation sequences.
- Random blink interval: 3–6 seconds.
- Awake touch triggers Left Wink.

### Sleep and wake

- 15 seconds of awake inactivity starts the drowsy/sleep transition.
- Sleeping animation alternates Sleep 2 and Sleep 3.
- Touch while drowsy or sleeping starts Wake 1 → Wake 2 → Wake 3 → Happy.
- If untouched, automatic wake begins after 25 seconds in the sleeping state.
- Touches during WAKING are ignored to prevent restarting the wake sequence.

These are animation/behaviour states; they do not place the ESP8266 into hardware deep sleep.

### ADXL345 knockout and recovery

- Direct register access; no separate ADXL345 library is required.
- Device ID check expects `0xE5`.
- Sensor configured for 100 Hz output data rate and full-resolution ±2g mode.
- Startup natural-orientation calibration averages 64 samples at 20 ms intervals.
- Keep the device still in its intended natural resting orientation during calibration.
- Filtered gravity-vector comparison detects sustained displacement beyond 45° for 180 ms.
- Return within 15° of the calibrated reference for 650 ms confirms recovery.
- Recovery sequence: Recovery 1 (500 ms) → Recovery 2 (500 ms) → Happy.
- Confirmed displacement overrides normal behaviour and touch input.

Thresholds are initial prototype settings in `Config.h`. Revalidate them after changing sensor mounting or enclosure orientation.

## Startup and failure handling

At reset, the firmware checks the ADXL345 device ID and calibrates the reference orientation. A successful calibration prints:

`ADXL345: calibration complete. Natural position stored.`

If the sensor is absent or calibration fails, OLED, touch, and serial input remain available, but automatic displacement detection is disabled.

## Serial Monitor

Set the Serial Monitor to **115200 baud**.

| Character | Action |
|---|---|
| `t` | Simulate touch |
| `l` | Trigger left-wink test event |
| `r` | Trigger right-wink test event |
| `k` | Preview Knocked Out bitmap |
| `1` | Preview Recovery 1 bitmap |
| `2` | Preview Recovery 2 bitmap |
| `h` | Return to Happy expression |

## Build notes

1. Open `CuboBot_Firmware.ino` in Arduino IDE.
2. Install the U8g2 library.
3. Select the Wemos D1 mini / appropriate ESP8266 board.
4. Verify wiring and upload.
5. Open Serial Monitor at 115200 baud and keep the device still in its intended resting orientation during calibration.

The firmware has been exercised on the current breadboard prototype by the project author. This does not constitute validation of a final PCB, battery/power system, or enclosure. Re-test after hardware or mounting changes.
