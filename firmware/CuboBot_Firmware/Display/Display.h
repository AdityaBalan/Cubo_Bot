#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "../Expressions/Expressions.h"

class Display {
public:
  Display();
  void begin();
  void show(Expression expression);
  void clear();
private:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
};
