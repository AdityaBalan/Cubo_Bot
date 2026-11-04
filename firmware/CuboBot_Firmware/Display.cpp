#include "Display.h"
Display::Display() : u8g2(U8G2_R0, U8X8_PIN_NONE) {}
void Display::begin() { u8g2.begin(); clear(); }
void Display::show(Expression expression) {
  const ExpressionAsset& asset = getExpressionAsset(expression);
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, asset.width, asset.height, asset.bitmap);
  u8g2.sendBuffer();
}
void Display::clear() { u8g2.clearBuffer(); u8g2.sendBuffer(); }
