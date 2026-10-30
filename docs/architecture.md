# Firmware Architecture

## Overview

The firmware separates display rendering, bitmap assets, animation sequencing, behaviour decisions, and input acquisition. This keeps hardware-specific input drivers replaceable without coupling them to the behaviour state machine.

## Runtime flow

```text
Serial input adapter
        |
        v
   InputManager
        |
        v
   BehaviourFSM <------ millis()
        |
        v
 AnimationController
        |
        v
     Display
        |
        v
 SH1106 OLED
```

## Modules

### Display
Owns the U8g2 SH1106 display object and draws the bitmap associated with an expression. It does not decide when an expression should be shown.

### Expressions
Defines the expression enumeration, dimensions (128×64), bitmap assets, and expression-to-bitmap lookup. The bitmap arrays are stored in program memory using `PROGMEM`.

### Animation
Runs a sequence of expression frames with per-frame durations. It supports one-shot and looping sequences and advances from the main loop using elapsed `millis()` time.

### Behaviour
Implements four states:
- **AWAKE:** happy idle, automatic blink, and response to simulated interaction.
- **DROWSY:** short transition expression before sleep.
- **SLEEPING:** looping sleep animation.
- **WAKING:** wake animation before returning to awake.

The state machine selects animation sequences; it does not render bitmap data itself.

### Input
Converts incoming Serial characters into normalized events. This is a temporary development adapter. Future TTP223 and ADXL345 drivers should emit the same event types so the behaviour layer can remain independent of sensor implementation.

## Timing and memory
- Animation progression is non-blocking and based on `millis()`.
- Expression bitmaps are stored in flash with `PROGMEM`.
- Runtime animation state uses fixed-size/static data structures; no dynamic allocation is required for frame sequencing.

## Extension points
1. Replace or extend the Serial adapter with touch and accelerometer event producers.
2. Add sensor debouncing, gesture thresholds, and event prioritization.
3. Add a power manager only after the battery/regulator architecture is finalized.
