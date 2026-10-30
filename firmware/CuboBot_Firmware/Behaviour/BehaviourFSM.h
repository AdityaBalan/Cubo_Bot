#pragma once
#include <Arduino.h>
#include "../Animation/AnimationController.h"
#include "../Input/InputManager.h"
enum class BehaviourState : uint8_t { AWAKE, DROWSY, SLEEPING, WAKING };
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
  uint32_t lastInteractionAt, nextBlinkAt;
  void enterAwake(uint32_t now);
  void enterDrowsy(uint32_t now);
  void enterSleeping(uint32_t now);
  void enterWaking(uint32_t now);
  void scheduleNextBlink(uint32_t now);
  void triggerBlink(uint32_t now);
  void triggerWink(Expression expression, uint32_t now);
};
