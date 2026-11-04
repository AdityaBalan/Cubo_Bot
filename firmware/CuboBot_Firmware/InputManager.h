#pragma once
#include <Arduino.h>

enum class InputEventType : uint8_t {
  TOUCH,
  DOUBLE_TAP,
  SHAKE,
  TILT_LEFT,
  TILT_RIGHT,
  DISPLACED,
  RETURNED_UPRIGHT,
  PREVIEW_KNOCKED_OUT,
  PREVIEW_RECOVERY_1,
  PREVIEW_RECOVERY_2,
  PREVIEW_HAPPY
};

struct InputEvent { InputEventType type; };

class InputManager {
public:
  void begin();
  bool poll(InputEvent& event);
  bool accelerometerReady() const;

private:
  bool rawTouchState = false;
  bool stableTouchState = false;
  uint32_t rawStateChangedAt = 0;

  bool accelReady = false;
  bool accelDisplaced = false;
  bool accelFilterInitialized = false;
  bool knockoutCandidateActive = false;
  bool recoveryCandidateActive = false;
  uint32_t lastAccelSampleAt = 0;
  uint32_t knockoutCandidateAt = 0;
  uint32_t recoveryCandidateAt = 0;
  float referenceX = 0.0f;
  float referenceY = 0.0f;
  float referenceZ = 1.0f;
  float filteredX = 0.0f;
  float filteredY = 0.0f;
  float filteredZ = 1.0f;

  bool initializeAccelerometer();
  bool readAcceleration(float& x, float& y, float& z);
  bool calibrateNaturalPosition();
  bool pollAccelerometer(InputEvent& event);
  bool pollTouch(InputEvent& event);
  bool pollSerial(InputEvent& event);
};
