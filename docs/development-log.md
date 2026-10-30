# Development Log

## 2026-10-31 — OLED firmware foundation

**Milestone:** Modular expression and behaviour firmware running on the Wemos D1 Mini (ESP8266).

### Completed
- SH1106 128×64 OLED initialized through U8g2.
- Nine expression assets incorporated: Happy, Blink Partial, Blink, Left Wink, Right Wink, Sleep 1, Sleep 2, Sleep 3, and Wake.
- Display, expression, animation, behaviour, and input code separated into modules.
- Non-blocking animation progression implemented using `millis()`.
- Awake, drowsy, sleeping, and waking states implemented.
- Serial input simulation added for touch and tilt events.
- Project developer confirmed the firmware is working on the Wemos D1 Mini.

### Current test interface
- Serial Monitor: 115200 baud.
- `t`: touch event.
- `l`: left tilt event.
- `r`: right tilt event.

### Not yet completed
- Physical TTP223 integration and validation.
- ADXL345 integration and gesture calibration.
- Battery, charger, regulator, and power-path design.
- Deep-sleep wake strategy and current measurement.
- 3D enclosure design and fit validation.

### Next milestone
Integrate TTP223 first, validate the electrical interface and event handling, then integrate the ADXL345. Keep the working OLED/animation firmware as the known-good baseline during sensor bring-up.
