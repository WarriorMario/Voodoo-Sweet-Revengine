#pragma once

enum class ButtonCode
{
  // Keyboard
  ENTER = 0x0D,
  ZERO = 0x30,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  SPACE = 0x20,
  SHIFT = 0x10,
  CONTROL,
  ESC = 0x1B,
  LEFT = 0x25,
  UP,
  RIGHT,
  DOWN,
  A = 65,
  B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  // Controller
  GAMEPAD_DPAD_UP = 0x0001 << 8,
  GAMEPAD_DPAD_DOWN = 0x0002 << 8,
  GAMEPAD_DPAD_LEFT = 0x0004 << 8,
  GAMEPAD_DPAD_RIGHT = 0x0008 << 8,
  GAMEPAD_START = 0x0010 << 8,
  GAMEPAD_BACK = 0x0020 << 8,
  GAMEPAD_LEFT_THUMB = 0x0040 << 8,
  GAMEPAD_RIGHT_THUMB = 0x0080 << 8,
  GAMEPAD_LEFT_SHOULDER = 0x0100 << 8,
  GAMEPAD_RIGHT_SHOULDER = 0x0200 << 8,
  GAMEPAD_A = 0x1000 << 8,
  GAMEPAD_B = 0x2000 << 8,
  GAMEPAD_X = 0x4000 << 8,
  GAMEPAD_Y = 0x8000 << 8,
  GAMEPAD_LEFT_TRIGGER = 0x10000 << 8,
  GAMEPAD_RIGHT_TRIGGER = 0x20000 << 8,
  // Mouse
  LEFT_MOUSE,
  RIGHT_MOUSE,
};

enum class AxisCode
{
  Left,
  Right
};