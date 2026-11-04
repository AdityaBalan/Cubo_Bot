#pragma once
#include <Arduino.h>

constexpr uint32_t SERIAL_BAUD = 115200;

// Existing TTP223 configuration
constexpr uint8_t TOUCH_PIN = D5; // Wemos D1 mini: D5 = GPIO14
constexpr uint16_t TOUCH_DEBOUNCE_MS = 35;

// Existing behaviour timing
constexpr uint32_t IDLE_TIMEOUT_MS = 15000;
constexpr uint32_t AUTO_WAKE_TIMEOUT_MS = 25000;
constexpr uint32_t BLINK_MIN_INTERVAL_MS = 3000;
constexpr uint32_t BLINK_MAX_INTERVAL_MS = 6000;
constexpr uint16_t BLINK_PARTIAL_MS = 140;
constexpr uint16_t BLINK_CLOSED_MS = 140;
constexpr uint16_t WINK_MS = 400;
constexpr uint16_t SLEEP_TRANSITION_MS = 450;
constexpr uint16_t SLEEP_FRAME_MS = 900;
constexpr uint16_t WAKE_FRAME_MS = 450;

// ADXL345 direct-I2C configuration
constexpr uint8_t ADXL345_ADDRESS = 0x53; // SDO tied LOW
constexpr uint8_t ADXL345_DEVICE_ID = 0xE5;
constexpr uint16_t ACCEL_SAMPLE_INTERVAL_MS = 20; // 50 Hz polling
constexpr uint8_t ACCEL_CALIBRATION_SAMPLES = 64;
constexpr uint16_t ACCEL_CALIBRATION_INTERVAL_MS = 20;
constexpr float ACCEL_SCALE_G_PER_LSB = 0.0039f; // Full-resolution, +/-2g
constexpr float ACCEL_MIN_VALID_MAGNITUDE_G = 0.75f;
constexpr float ACCEL_MAX_VALID_MAGNITUDE_G = 1.25f;
constexpr float ACCEL_KNOCKOUT_COS_THRESHOLD = 0.7071f; // cos(45 degrees)
constexpr float ACCEL_RECOVERY_COS_THRESHOLD = 0.9659f; // cos(15 degrees)
constexpr uint16_t ACCEL_KNOCKOUT_CONFIRM_MS = 180;
constexpr uint16_t ACCEL_RECOVERY_CONFIRM_MS = 650;
constexpr float ACCEL_FILTER_ALPHA = 0.20f;
