#include "InputManager.h"
void InputManager::begin() {}
bool InputManager::poll(InputEvent& event) {
  if (!Serial.available()) return false;
  const char c=static_cast<char>(Serial.read());
  switch(c) {
    case 't': case 'T': event.type=InputEventType::TOUCH; return true;
    case 'l': case 'L': event.type=InputEventType::TILT_LEFT; return true;
    case 'r': case 'R': event.type=InputEventType::TILT_RIGHT; return true;
    default: return false;
  }
}
