#pragma once
#include <Arduino.h>
constexpr uint8_t EXPRESSION_WIDTH = 128;
constexpr uint8_t EXPRESSION_HEIGHT = 64;
enum class Expression : uint8_t { HAPPY, BLINK_PARTIAL, BLINK, WINK_LEFT, WINK_RIGHT, SLEEP_1, SLEEP_2, SLEEP_3, WAKE_1, WAKE_2, WAKE_3, KNOCKED_OUT, RECOVERY_1, RECOVERY_2 };
struct ExpressionAsset { const uint8_t* bitmap; uint8_t width; uint8_t height; };
const ExpressionAsset& getExpressionAsset(Expression expression);
