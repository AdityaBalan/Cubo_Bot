#include "InputManager.h"
#include "Config.h"
#include <Wire.h>
#include <math.h>

namespace {
constexpr uint8_t REG_DEVID = 0x00;
constexpr uint8_t REG_BW_RATE = 0x2C;
constexpr uint8_t REG_POWER_CTL = 0x2D;
constexpr uint8_t REG_DATA_FORMAT = 0x31;
constexpr uint8_t REG_DATAX0 = 0x32;

bool writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool readRegister(uint8_t reg, uint8_t& value) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)ADXL345_ADDRESS, (uint8_t)1) != 1) return false;
  value = Wire.read();
  return true;
}

float vectorMagnitude(float x, float y, float z) {
  return sqrtf(x * x + y * y + z * z);
}
}

void InputManager::begin() {
  pinMode(TOUCH_PIN, INPUT); // TTP223 is active-HIGH
  const bool initialState = (digitalRead(TOUCH_PIN) == HIGH);
  rawTouchState = initialState;
  stableTouchState = initialState;
  rawStateChangedAt = millis();

  Serial.println();
  Serial.println(F("ADXL345: checking I2C device at 0x53..."));
  accelReady = initializeAccelerometer();
  if (!accelReady) {
    Serial.println(F("ADXL345: not detected/configuration failed; touch and serial remain available."));
    return;
  }

  Serial.println(F("ADXL345: keep Cubo in its natural upright position and hold it still."));
  if (!calibrateNaturalPosition()) {
    accelReady = false;
    Serial.println(F("ADXL345: calibration failed (gravity magnitude outside 0.75-1.25g)."));
    Serial.println(F("ADXL345: displacement detection disabled; touch and serial remain available."));
    return;
  }
  Serial.println(F("ADXL345: calibration complete. Natural position stored."));
}

bool InputManager::initializeAccelerometer() {
  uint8_t deviceId = 0;
  if (!readRegister(REG_DEVID, deviceId) || deviceId != ADXL345_DEVICE_ID) {
    Serial.print(F("ADXL345: unexpected device ID 0x"));
    Serial.println(deviceId, HEX);
    return false;
  }

  // 100 Hz output data rate; full-resolution +/-2g; measurement mode.
  if (!writeRegister(REG_BW_RATE, 0x0A)) return false;
  if (!writeRegister(REG_DATA_FORMAT, 0x08)) return false;
  if (!writeRegister(REG_POWER_CTL, 0x08)) return false;
  delay(20);

  Serial.print(F("ADXL345: device ID 0x"));
  Serial.println(deviceId, HEX);
  return true;
}

bool InputManager::readAcceleration(float& x, float& y, float& z) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(REG_DATAX0);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)ADXL345_ADDRESS, (uint8_t)6) != 6) return false;

  const uint16_t xBits = (uint16_t)Wire.read() | ((uint16_t)Wire.read() << 8);
  const uint16_t yBits = (uint16_t)Wire.read() | ((uint16_t)Wire.read() << 8);
  const uint16_t zBits = (uint16_t)Wire.read() | ((uint16_t)Wire.read() << 8);
  const int16_t rawX = (int16_t)xBits;
  const int16_t rawY = (int16_t)yBits;
  const int16_t rawZ = (int16_t)zBits;

  x = rawX * ACCEL_SCALE_G_PER_LSB;
  y = rawY * ACCEL_SCALE_G_PER_LSB;
  z = rawZ * ACCEL_SCALE_G_PER_LSB;
  return true;
}

bool InputManager::calibrateNaturalPosition() {
  float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
  for (uint8_t i = 0; i < ACCEL_CALIBRATION_SAMPLES; ++i) {
    float x, y, z;
    if (!readAcceleration(x, y, z)) return false;
    sumX += x;
    sumY += y;
    sumZ += z;
    delay(ACCEL_CALIBRATION_INTERVAL_MS);
  }

  const float avgX = sumX / ACCEL_CALIBRATION_SAMPLES;
  const float avgY = sumY / ACCEL_CALIBRATION_SAMPLES;
  const float avgZ = sumZ / ACCEL_CALIBRATION_SAMPLES;
  const float magnitude = vectorMagnitude(avgX, avgY, avgZ);
  if (magnitude < ACCEL_MIN_VALID_MAGNITUDE_G || magnitude > ACCEL_MAX_VALID_MAGNITUDE_G) {
    Serial.print(F("ADXL345: calibration magnitude = "));
    Serial.print(magnitude, 3);
    Serial.println(F("g"));
    return false;
  }

  referenceX = avgX / magnitude;
  referenceY = avgY / magnitude;
  referenceZ = avgZ / magnitude;
  filteredX = avgX;
  filteredY = avgY;
  filteredZ = avgZ;
  accelFilterInitialized = true;
  lastAccelSampleAt = millis();
  return true;
}

bool InputManager::poll(InputEvent& event) {
  // Physical displacement has priority over touch and serial test inputs.
  if (pollAccelerometer(event)) return true;
  if (pollTouch(event)) return true;
  return pollSerial(event);
}

bool InputManager::pollAccelerometer(InputEvent& event) {
  if (!accelReady) return false;
  const uint32_t now = millis();
  if (static_cast<uint32_t>(now - lastAccelSampleAt) < ACCEL_SAMPLE_INTERVAL_MS) return false;
  lastAccelSampleAt = now;

  float x, y, z;
  if (!readAcceleration(x, y, z)) return false;

  const float magnitude = vectorMagnitude(x, y, z);
  // Ignore high-dynamic-acceleration samples (e.g. a quick shake/impact).
  // This detector is intended to classify a settled orientation, not impacts.
  if (magnitude < ACCEL_MIN_VALID_MAGNITUDE_G || magnitude > ACCEL_MAX_VALID_MAGNITUDE_G) {
    knockoutCandidateActive = false;
    recoveryCandidateActive = false;
    return false;
  }

  if (!accelFilterInitialized) {
    filteredX = x; filteredY = y; filteredZ = z;
    accelFilterInitialized = true;
  } else {
    filteredX += ACCEL_FILTER_ALPHA * (x - filteredX);
    filteredY += ACCEL_FILTER_ALPHA * (y - filteredY);
    filteredZ += ACCEL_FILTER_ALPHA * (z - filteredZ);
  }

  const float filteredMagnitude = vectorMagnitude(filteredX, filteredY, filteredZ);
  if (filteredMagnitude < 0.001f) return false;
  const float dot = (referenceX * filteredX + referenceY * filteredY + referenceZ * filteredZ) / filteredMagnitude;

  if (!accelDisplaced) {
    recoveryCandidateActive = false;
    if (dot < ACCEL_KNOCKOUT_COS_THRESHOLD) {
      if (!knockoutCandidateActive) {
        knockoutCandidateActive = true;
        knockoutCandidateAt = now;
      } else if (static_cast<uint32_t>(now - knockoutCandidateAt) >= ACCEL_KNOCKOUT_CONFIRM_MS) {
        accelDisplaced = true;
        knockoutCandidateActive = false;
        event.type = InputEventType::DISPLACED;
        Serial.println(F("ADXL345: displacement detected -> KNOCKED OUT"));
        return true;
      }
    } else {
      knockoutCandidateActive = false;
    }
  } else {
    knockoutCandidateActive = false;
    if (dot > ACCEL_RECOVERY_COS_THRESHOLD) {
      if (!recoveryCandidateActive) {
        recoveryCandidateActive = true;
        recoveryCandidateAt = now;
      } else if (static_cast<uint32_t>(now - recoveryCandidateAt) >= ACCEL_RECOVERY_CONFIRM_MS) {
        accelDisplaced = false;
        recoveryCandidateActive = false;
        event.type = InputEventType::RETURNED_UPRIGHT;
        Serial.println(F("ADXL345: natural position confirmed -> RECOVERY"));
        return true;
      }
    } else {
      recoveryCandidateActive = false;
    }
  }
  return false;
}

bool InputManager::pollTouch(InputEvent& event) {
  const uint32_t now = millis();
  const bool reading = (digitalRead(TOUCH_PIN) == HIGH);

  if (reading != rawTouchState) {
    rawTouchState = reading;
    rawStateChangedAt = now;
  }

  if (rawTouchState != stableTouchState &&
      static_cast<uint32_t>(now - rawStateChangedAt) >= TOUCH_DEBOUNCE_MS) {
    stableTouchState = rawTouchState;
    if (stableTouchState) {
      event.type = InputEventType::TOUCH;
      Serial.println(F("TTP223: TOUCH DETECTED"));
      return true;
    }
    Serial.println(F("TTP223: TOUCH RELEASED"));
  }
  return false;
}

bool InputManager::pollSerial(InputEvent& event) {
  while (Serial.available()) {
    const char c = static_cast<char>(Serial.read());
    switch (c) {
      case 't': case 'T': event.type = InputEventType::TOUCH; return true;
      case 'l': case 'L': event.type = InputEventType::TILT_LEFT; return true;
      case 'r': case 'R': event.type = InputEventType::TILT_RIGHT; return true;
      case 'k': case 'K': event.type = InputEventType::PREVIEW_KNOCKED_OUT; return true;
      case '1': event.type = InputEventType::PREVIEW_RECOVERY_1; return true;
      case '2': event.type = InputEventType::PREVIEW_RECOVERY_2; return true;
      case 'h': case 'H': event.type = InputEventType::PREVIEW_HAPPY; return true;
      default: break;
    }
  }
  return false;
}

bool InputManager::accelerometerReady() const { return accelReady; }
