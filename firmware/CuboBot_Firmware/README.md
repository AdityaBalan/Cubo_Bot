# Cubo Bot – ADXL345 Knockout / Recovery Integration

## Hardware baseline
- Wemos D1 mini / ESP8266
- 1.3-inch 128x64 SH1106 OLED using U8g2
- OLED: VCC→3V3, GND→GND, SDA→D2/GPIO4, SCL→D1/GPIO5
- ADXL345: VCC→3V3, GND→GND, SDA→D2/GPIO4, SCL→D1/GPIO5
- ADXL345 address: `0x53` (SDO LOW); CS must select I2C mode (HIGH)
- TTP223: VCC→3V3, GND→GND, OUT/SIG→D5/GPIO14

## What this build adds
- Direct ADXL345 register access using `Wire.h`; no Adafruit libraries required.
- Device ID verification (`0xE5`), 100 Hz sensor ODR, full-resolution ±2g mode.
- Startup natural-position calibration using 64 samples. Keep Cubo upright and still during startup calibration (about 1.3 seconds).
- Three-axis gravity-vector comparison, so displacement can be detected in any direction.
- Low-pass filtering and acceleration-magnitude validation to reject most transient shakes/impacts.
- Displacement beyond 45 degrees for 180 ms enters `KNOCKED_OUT`.
- Return within 15 degrees for 650 ms enters `RECOVERING`.
- Recovery sequence: Recovery 1 (500 ms) → Recovery 2 (500 ms) → Happy.
- Displacement has priority over awake, drowsy, sleeping, waking and recovery states.
- Touch and ordinary commands are ignored while knocked out or recovering.

## Existing behaviour retained
- SH1106 OLED and all 14 bitmap assets.
- TTP223 active-HIGH input on D5 with 35 ms debounce.
- Touch-triggered Left Wink while awake.
- Touch-triggered Wake 1 → Wake 2 → Wake 3 while sleeping/drowsy.
- Touches during WAKING are ignored; wake animation cannot be restarted.
- Random blink, 15-second awake inactivity timeout, sleep animation and 25-second automatic wake.
- Serial test commands and static bitmap preview commands.

## Startup calibration
Power up/reset Cubo while it is in its intended natural upright position and hold it still until Serial Monitor reports:
`ADXL345: calibration complete. Natural position stored.`

The reference is recalibrated on every reset. If the ADXL345 is not detected or calibration fails, the OLED, TTP223 and serial behaviour continue to work, but automatic displacement detection is disabled.

## Serial Monitor
115200 baud.
- `t`: simulated touch
- `l`: left wink test
- `r`: right wink test
- `k`: preview Knocked Out bitmap (static preview only)
- `1`: preview Recovery 1 bitmap
- `2`: preview Recovery 2 bitmap
- `h`: return to Happy Face

## Important validation note
The firmware package has not been compiled or hardware-tested as a complete integrated build in this environment. Validate first with Cubo upright during startup, then test controlled tilts on a soft surface. Thresholds and timing are initial values and can be tuned after observing the mounted sensor behaviour.
