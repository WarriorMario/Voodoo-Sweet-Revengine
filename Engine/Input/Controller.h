#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "MatrixDef.h"
class Controller
{
public:
  Controller()
  {}
  void Poll();
  bool ButtonIsPressed(size_t code);
  bool ButtonIsReleased(size_t code);
  bool ButtonIsDown(size_t code);
  Vec2 LeftStick();
  Vec2 RightStick();
  Vec2 Trigger();
public:
  XINPUT_GAMEPAD controller;
  XINPUT_GAMEPAD last_controller;
  unsigned char trigger_button_threshold = 200;

  size_t ID;
};