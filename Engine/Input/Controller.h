#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "Vec2.h"
class Controller
{
public:
  Controller()
  {}
  void Poll();
  bool ButtonIsPressed(size_t code);
  Vec2 LeftStick();
  Vec2 RightStick();
  Vec2 Trigger();
public:
  XINPUT_GAMEPAD controller;
  unsigned char trigger_button_threshold = 200;

  size_t ID;
};