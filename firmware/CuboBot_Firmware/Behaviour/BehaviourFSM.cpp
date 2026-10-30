#include "BehaviourFSM.h"
#include "../Config.h"
namespace {
const AnimationFrame BLINK[]={{Expression::BLINK_PARTIAL,BLINK_PARTIAL_MS},{Expression::BLINK,BLINK_CLOSED_MS},{Expression::HAPPY,1}};
const AnimationFrame WINK_L[]={{Expression::WINK_LEFT,WINK_MS},{Expression::HAPPY,1}};
const AnimationFrame WINK_R[]={{Expression::WINK_RIGHT,WINK_MS},{Expression::HAPPY,1}};
const AnimationFrame DROWSY[]={{Expression::SLEEP_1,DROWSY_MS}};
const AnimationFrame WAKE[]={{Expression::WAKE,WAKE_MS},{Expression::HAPPY,1}};
const AnimationFrame SLEEP_LOOP[]={{Expression::SLEEP_2,SLEEP_FRAME_MS},{Expression::SLEEP_3,SLEEP_FRAME_MS}};
}
BehaviourFSM::BehaviourFSM(AnimationController& a)
 : animation(a),currentState(BehaviourState::AWAKE),lastInteractionAt(0),nextBlinkAt(0) {}
void BehaviourFSM::begin(uint32_t now) { lastInteractionAt=now; enterAwake(now); }
void BehaviourFSM::handleEvent(const InputEvent& event,uint32_t now) {
  switch(event.type) {
    case InputEventType::TOUCH:
      lastInteractionAt=now;
      if(currentState==BehaviourState::SLEEPING || currentState==BehaviourState::DROWSY) enterWaking(now);
      else if(currentState==BehaviourState::AWAKE) triggerWink(Expression::WINK_LEFT,now);
      break;
    case InputEventType::TILT_LEFT:
      if(currentState==BehaviourState::AWAKE) { lastInteractionAt=now; triggerWink(Expression::WINK_LEFT,now); }
      break;
    case InputEventType::TILT_RIGHT:
      if(currentState==BehaviourState::AWAKE) { lastInteractionAt=now; triggerWink(Expression::WINK_RIGHT,now); }
      break;
    case InputEventType::DOUBLE_TAP: case InputEventType::SHAKE:
      lastInteractionAt=now; break;
  }
}
void BehaviourFSM::update(uint32_t now) {
  switch(currentState) {
    case BehaviourState::AWAKE:
      if(static_cast<uint32_t>(now-lastInteractionAt)>=IDLE_TIMEOUT_MS) { enterDrowsy(now); return; }
      if(!animation.isPlaying() && static_cast<int32_t>(now-nextBlinkAt)>=0) triggerBlink(now);
      break;
    case BehaviourState::DROWSY: if(!animation.isPlaying()) enterSleeping(now); break;
    case BehaviourState::SLEEPING: break;
    case BehaviourState::WAKING: if(!animation.isPlaying()) enterAwake(now); break;
  }
}
void BehaviourFSM::enterAwake(uint32_t now) { currentState=BehaviourState::AWAKE; lastInteractionAt=now; animation.show(Expression::HAPPY); scheduleNextBlink(now); }
void BehaviourFSM::enterDrowsy(uint32_t now) { currentState=BehaviourState::DROWSY; animation.play(DROWSY,1,now,false); }
void BehaviourFSM::enterSleeping(uint32_t now) { currentState=BehaviourState::SLEEPING; animation.play(SLEEP_LOOP,2,now,true); }
void BehaviourFSM::enterWaking(uint32_t now) { currentState=BehaviourState::WAKING; animation.play(WAKE,2,now,false); }
void BehaviourFSM::scheduleNextBlink(uint32_t now) { nextBlinkAt=now+random(BLINK_MIN_INTERVAL_MS,BLINK_MAX_INTERVAL_MS+1); }
void BehaviourFSM::triggerBlink(uint32_t now) { animation.play(BLINK,3,now,false); scheduleNextBlink(now); }
void BehaviourFSM::triggerWink(Expression e,uint32_t now) { if(e==Expression::WINK_RIGHT) animation.play(WINK_R,2,now,false); else animation.play(WINK_L,2,now,false); scheduleNextBlink(now); }
BehaviourState BehaviourFSM::state() const { return currentState; }
