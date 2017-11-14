#pragma once
#include "ButtonCodes.h"
#include "Vec2.h"
class Input
{
public:
  Input(class Keyboard& kbd, class Mouse& mouse);
  bool IsPressed(ButtonCode code, int device_ID = 0);
  Vec2 GetAxis(AxisCode code, int device_ID);
  Vec2 MousePos();
  void Poll();

  class Keyboard& kbd;
  class Mouse& mouse;
  class Controller* controllers;
};