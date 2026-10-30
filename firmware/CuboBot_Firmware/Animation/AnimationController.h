#pragma once
#include <Arduino.h>
#include "../Expressions/Expressions.h"
#include "../Display/Display.h"
struct AnimationFrame { Expression expression; uint16_t durationMs; };
class AnimationController {
public:
  explicit AnimationController(Display& display);
  void play(const AnimationFrame* frames, uint8_t count, uint32_t now, bool loop=false);
  void show(Expression expression);
  void stop();
  void update(uint32_t now);
  bool isPlaying() const;
  Expression currentExpression() const;
private:
  Display& display;
  const AnimationFrame* frames;
  uint8_t count, index;
  uint32_t frameStartedAt;
  bool looping, playing;
  Expression current;
};
