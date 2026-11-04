#include "BehaviourFSM.h"
#include "Config.h"

namespace {
const AnimationFrame BLINK[] = {
  {Expression::BLINK_PARTIAL, BLINK_PARTIAL_MS},
  {Expression::BLINK, BLINK_CLOSED_MS},
  {Expression::HAPPY, 1}
};
const AnimationFrame WINK_L[] = {
  {Expression::WINK_LEFT, WINK_MS},
  {Expression::HAPPY, 1}
};
const AnimationFrame WINK_R[] = {
  {Expression::WINK_RIGHT, WINK_MS},
  {Expression::HAPPY, 1}
};
const AnimationFrame SLEEP_ENTRY[] = {
  {Expression::SLEEP_1, SLEEP_TRANSITION_MS},
  {Expression::SLEEP_2, 1}
};
const AnimationFrame SLEEP_LOOP[] = {
  {Expression::SLEEP_2, SLEEP_FRAME_MS},
  {Expression::SLEEP_3, SLEEP_FRAME_MS}
};
const AnimationFrame WAKE_SEQUENCE[] = {
  {Expression::WAKE_1, WAKE_FRAME_MS},
  {Expression::WAKE_2, WAKE_FRAME_MS},
  {Expression::WAKE_3, WAKE_FRAME_MS},
  {Expression::HAPPY, 1}
};
const AnimationFrame RECOVERY_SEQUENCE[] = {
  {Expression::RECOVERY_1, 500},
  {Expression::RECOVERY_2, 500},
  {Expression::HAPPY, 1}
};
}

BehaviourFSM::BehaviourFSM(AnimationController& a)
  : animation(a), currentState(BehaviourState::AWAKE),
    lastInteractionAt(0), sleepStartedAt(0), nextBlinkAt(0) {}

void BehaviourFSM::begin(uint32_t now) {
  lastInteractionAt = now;
  enterAwake(now);
}

void BehaviourFSM::handleEvent(const InputEvent& event, uint32_t now) {
  // Physical displacement is the highest-priority event in every state.
  if (event.type == InputEventType::DISPLACED) {
    if (currentState != BehaviourState::KNOCKED_OUT) enterKnockedOut();
    return;
  }

  // Only the accelerometer's confirmed return-to-natural event can leave KO.
  if (currentState == BehaviourState::KNOCKED_OUT) {
    if (event.type == InputEventType::RETURNED_UPRIGHT) enterRecovering(now);
    return;
  }

  // Recovery is atomic: ignore touch, serial preview, and ordinary inputs.
  if (currentState == BehaviourState::RECOVERING) return;

  // A touch cannot restart the wake sequence once WAKING has begun.
  if (currentState == BehaviourState::WAKING) return;

  if (currentState == BehaviourState::DROWSY ||
      currentState == BehaviourState::SLEEPING) {
    if (event.type == InputEventType::TOUCH) {
      lastInteractionAt = now;
      enterWaking(now);
    }
    // Winks and preview commands never wake Cubo.
    return;
  }

  switch (event.type) {
    case InputEventType::TOUCH:
      lastInteractionAt = now;
      triggerWink(Expression::WINK_LEFT, now);
      break;
    case InputEventType::TILT_LEFT:
      // Retained as a serial test event; physical accelerometer tilts now
      // control the Knocked Out / Recovery interaction instead of winks.
      lastInteractionAt = now;
      triggerWink(Expression::WINK_LEFT, now);
      break;
    case InputEventType::TILT_RIGHT:
      lastInteractionAt = now;
      triggerWink(Expression::WINK_RIGHT, now);
      break;
    case InputEventType::PREVIEW_KNOCKED_OUT:
      animation.show(Expression::KNOCKED_OUT);
      lastInteractionAt = now;
      break;
    case InputEventType::PREVIEW_RECOVERY_1:
      animation.show(Expression::RECOVERY_1);
      lastInteractionAt = now;
      break;
    case InputEventType::PREVIEW_RECOVERY_2:
      animation.show(Expression::RECOVERY_2);
      lastInteractionAt = now;
      break;
    case InputEventType::PREVIEW_HAPPY:
      animation.show(Expression::HAPPY);
      lastInteractionAt = now;
      scheduleNextBlink(now);
      break;
    case InputEventType::DOUBLE_TAP:
    case InputEventType::SHAKE:
    case InputEventType::DISPLACED:
    case InputEventType::RETURNED_UPRIGHT:
      break;
  }
}

void BehaviourFSM::update(uint32_t now) {
  switch (currentState) {
    case BehaviourState::AWAKE:
      if (static_cast<uint32_t>(now - lastInteractionAt) >= IDLE_TIMEOUT_MS) {
        enterDrowsy(now);
        return;
      }
      if (!animation.isPlaying() && static_cast<int32_t>(now - nextBlinkAt) >= 0) triggerBlink(now);
      break;

    case BehaviourState::DROWSY:
      if (!animation.isPlaying()) enterSleeping(now);
      break;

    case BehaviourState::SLEEPING:
      if (static_cast<uint32_t>(now - sleepStartedAt) >= AUTO_WAKE_TIMEOUT_MS) {
        enterWaking(now);
        return;
      }
      if (!animation.isPlaying()) animation.play(SLEEP_LOOP, 2, now, true);
      break;

    case BehaviourState::WAKING:
      if (!animation.isPlaying()) enterAwake(now);
      break;

    case BehaviourState::KNOCKED_OUT:
      // Hold the knockout bitmap until the accelerometer confirms upright.
      break;

    case BehaviourState::RECOVERING:
      if (!animation.isPlaying()) enterAwake(now);
      break;
  }
}

void BehaviourFSM::enterAwake(uint32_t now) {
  currentState = BehaviourState::AWAKE;
  lastInteractionAt = now;
  animation.show(Expression::HAPPY);
  scheduleNextBlink(now);
}

void BehaviourFSM::enterDrowsy(uint32_t now) {
  currentState = BehaviourState::DROWSY;
  animation.play(SLEEP_ENTRY, 2, now, false);
}

void BehaviourFSM::enterSleeping(uint32_t now) {
  currentState = BehaviourState::SLEEPING;
  sleepStartedAt = now;
  animation.play(SLEEP_LOOP, 2, now, true);
}

void BehaviourFSM::enterWaking(uint32_t now) {
  currentState = BehaviourState::WAKING;
  animation.play(WAKE_SEQUENCE, 4, now, false);
}

void BehaviourFSM::enterKnockedOut() {
  currentState = BehaviourState::KNOCKED_OUT;
  animation.show(Expression::KNOCKED_OUT);
  Serial.println(F("Cubo state: KNOCKED_OUT (touch and normal behaviour suspended)"));
}

void BehaviourFSM::enterRecovering(uint32_t now) {
  currentState = BehaviourState::RECOVERING;
  animation.play(RECOVERY_SEQUENCE, 3, now, false);
  Serial.println(F("Cubo state: RECOVERING -> RECOVERY 1 -> RECOVERY 2 -> HAPPY"));
}

void BehaviourFSM::scheduleNextBlink(uint32_t now) {
  nextBlinkAt = now + random(BLINK_MIN_INTERVAL_MS, BLINK_MAX_INTERVAL_MS + 1);
}

void BehaviourFSM::triggerBlink(uint32_t now) {
  animation.play(BLINK, 3, now, false);
  scheduleNextBlink(now);
}

void BehaviourFSM::triggerWink(Expression e, uint32_t now) {
  animation.play(e == Expression::WINK_RIGHT ? WINK_R : WINK_L, 2, now, false);
  scheduleNextBlink(now);
}

BehaviourState BehaviourFSM::state() const { return currentState; }
