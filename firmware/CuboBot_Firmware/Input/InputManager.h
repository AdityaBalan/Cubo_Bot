#pragma once
#include <Arduino.h>
enum class InputEventType : uint8_t { TOUCH, DOUBLE_TAP, SHAKE, TILT_LEFT, TILT_RIGHT };
struct InputEvent { InputEventType type; };
class InputManager {
public: void begin(); bool poll(InputEvent& event);
};
