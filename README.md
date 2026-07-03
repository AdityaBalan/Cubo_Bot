# Cubo

> An interactive ESP32-C3 powered desktop companion featuring expressive OLED animations, touch and motion interaction, rechargeable battery operation, and a custom 3D-printed enclosure.

> **Project Status:** 🚧 Under Active Development

---

## Overview

Cubo is a compact, portable desktop companion designed to bring personality to any workspace through expressive animations and interactive behaviors.

Inspired by the charm of minimalist digital companions, Cubo combines embedded electronics, custom firmware, and industrial design into a single handheld device. The project emphasizes reliability, smooth animations, efficient power management, and a polished user experience.

The long-term goal is to create a product-quality DIY companion that feels engaging, responsive, and thoughtfully engineered rather than simply functioning as another electronics prototype.

---

## Planned Features

- Expressive OLED face animations
- Smooth blinking and eye movement
- Touch-based interaction
- Motion-aware reactions using a 3-axis accelerometer
- Rechargeable LiPo battery with onboard charging
- Portable, fully enclosed design
- Low-power operation
- Modular firmware architecture
- Custom-designed 3D printed enclosure

---

## Hardware

| Component | Status |
|-----------|--------|
| ESP32-C3 Super Mini | ✅ |
| 1.3" SSD1306 OLED Display | ✅ |
| TTP223 Capacitive Touch Sensor | ✅ |
| ADXL345 Accelerometer | ✅ |
| TP4056 USB-C Charging Module | ✅ |
| 800 mAh LiPo Battery | ✅ |
| Custom 3D Printed Enclosure | ⏳ In Progress |

---

## Software Stack

- Arduino IDE/ESP-IDF
- ESP32 Arduino Framework
- U8g2 Graphics Library
- Git & GitHub

---

## Repository Structure

```
Cubo/
├── README.md
├── LICENSE
├── firmware/
├── hardware/
├── docs/
├── tests/
└── media/
```

This repository will evolve alongside the project, with documentation, CAD files, firmware, wiring diagrams, and testing results added as each development milestone is completed.

---

## Development Roadmap

### Phase 1 — Hardware Bring-up
- [x] Repository initialization
- [x] ESP32-C3 verification
- [ ] OLED initialization
- [ ] I²C validation

### Phase 2 — Display System
- [ ] Display driver
- [ ] Face renderer
- [ ] Blink animation
- [ ] Expression engine

### Phase 3 — Interaction
- [ ] Touch sensor integration
- [ ] Motion sensing
- [ ] Emotion state machine

### Phase 4 — Power System
- [ ] Battery integration
- [ ] Charging validation
- [ ] Battery monitoring
- [ ] Low-power optimization

### Phase 5 — Mechanical Design
- [ ] CAD enclosure
- [ ] 3D printing
- [ ] Assembly
- [ ] Final testing

---

## Project Goals

This project is being developed with an emphasis on:

- Reliability
- Maintainability
- Clean firmware architecture
- Professional documentation
- Reproducible hardware design
- Incremental development and testing

Every milestone is validated before introducing additional hardware or software complexity.

---

## License

This project is licensed under the MIT License.

---
