#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "Config.h"
#include "Display/Display.h"
#include "Animation/AnimationController.h"
#include "Input/InputManager.h"
#include "Behaviour/BehaviourFSM.h"

Display display;
AnimationController animation(display);
InputManager inputManager;
BehaviourFSM behaviour(animation);

void setup() {
  Serial.begin(SERIAL_BAUD);
  display.begin();
  inputManager.begin();
  randomSeed(analogRead(A0));
  behaviour.begin(millis());
  Serial.println(F("Cubo Bot firmware ready"));
  Serial.println(F("Commands: t=touch, l=left tilt, r=right tilt"));
}

void loop() {
  const uint32_t now = millis();
  InputEvent event;
  while (inputManager.poll(event)) {
    behaviour.handleEvent(event, now);
  }
  behaviour.update(now);
  animation.update(now);
}
