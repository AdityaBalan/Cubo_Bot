#include "AnimationController.h"
AnimationController::AnimationController(Display& d)
 : display(d), frames(nullptr), count(0), index(0), frameStartedAt(0),
   looping(false), playing(false), current(Expression::HAPPY) {}
void AnimationController::play(const AnimationFrame* sequence, uint8_t n, uint32_t now, bool loop) {
  if (!sequence || !n) { stop(); return; }
  frames=sequence; count=n; index=0; frameStartedAt=now; looping=loop; playing=true;
  current=frames[0].expression; display.show(current);
}
void AnimationController::show(Expression e) { stop(); current=e; display.show(e); }
void AnimationController::stop() { frames=nullptr; count=0; index=0; looping=false; playing=false; }
void AnimationController::update(uint32_t now) {
  if (!playing || !frames) return;
  if (static_cast<uint32_t>(now-frameStartedAt) < frames[index].durationMs) return;
  frameStartedAt=now;
  ++index;
  if (index>=count) {
    if (looping) index=0;
    else { playing=false; frames=nullptr; count=0; return; }
  }
  current=frames[index].expression;
  display.show(current);
}
bool AnimationController::isPlaying() const { return playing; }
Expression AnimationController::currentExpression() const { return current; }
