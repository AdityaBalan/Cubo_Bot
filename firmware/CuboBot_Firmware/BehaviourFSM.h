#pragma once
#include <Arduino.h>
#include "AnimationController.h"
#include "InputManager.h"

enum class BehaviourState : uint8_t {
  AWAKE,
  DROWSY,
  SLEEPING,
  WAKING,
  KNOCKED_OUT,
  RECOVERING
};

class BehaviourFSM {
public:
  explicit BehaviourFSM(AnimationController& animation);
  void begin(uint32_t now);
  void handleEvent(const InputEvent& event, uint32_t now);
  void update(uint32_t now);
  BehaviourState state() const;

private:
  AnimationController& animation;
  BehaviourState currentState;
  uint32_t lastInteractionAt, sleepStartedAt, nextBlinkAt;
  void enterAwake(uint32_t now);
  void enterDrowsy(uint32_t now);
  void enterSleeping(uint32_t now);
  void enterWaking(uint32_t now);
  void enterKnockedOut();
  void enterRecovering(uint32_t now);
  void scheduleNextBlink(uint32_t now);
  void triggerBlink(uint32_t now);
  void triggerWink(Expression expression, uint32_t now);
};
