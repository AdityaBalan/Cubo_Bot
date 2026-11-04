#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "Config.h"
#include "Display.h"
#include "AnimationController.h"
#include "InputManager.h"
#include "BehaviourFSM.h"

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

  Serial.println(F("Cubo Bot - ADXL345 knockout/recovery firmware ready"));
  Serial.println(F("Commands: t=touch/wake, l=left wink, r=right wink"));
  Serial.println(F("OLED preview (awake only): k=knocked out, 1=recovery 1, 2=recovery 2, h=happy"));
  Serial.println(F("Awake idle: 15s | Sleep auto-wake: 25s"));
  if (inputManager.accelerometerReady()) {
    Serial.println(F("ADXL345 displacement detection: ENABLED"));
  } else {
    Serial.println(F("ADXL345 displacement detection: DISABLED"));
  }
}

void loop() {
  const uint32_t now = millis();
  InputEvent event;
  while (inputManager.poll(event)) behaviour.handleEvent(event, now);
  behaviour.update(now);
  animation.update(now);
}
