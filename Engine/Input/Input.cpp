#include "Input.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"
const int NUM_CONTROLLERS = 4;
Input::Input(Keyboard & kbd, Mouse & mouse)
  :
  kbd(kbd),
  mouse(mouse)
{
  controllers = new Controller[NUM_CONTROLLERS];
  for(int i = 0; i < NUM_CONTROLLERS; ++i)
  {
    controllers[i].ID = i;
  }
}

bool Input::IsPressed(ButtonCode code, int device_ID)
{
  switch (code)
  {
    case ButtonCode::LEFT_MOUSE:
    {
      return mouse.LeftIsPressed();
    }
    case ButtonCode::RIGHT_MOUSE:
    {
      return mouse.RightIsPressed();
    }
    default:
    {
      if((size_t)code > 255)
      {
        controllers[device_ID].ButtonIsPressed((size_t)code);
      }
      else
      {
        kbd.KeyIsPressed((size_t)code);
      }
    }
  }
}

Vec2 Input::GetAxis(AxisCode code, int device_ID)
{
  switch(code)
  {
    case AxisCode::Left:
    {
      return controllers[device_ID].LeftStick();
    }
    case AxisCode::Right:
    {
      return controllers[device_ID].RightStick();
    }
  }
}

void Input::Poll()
{
  for(int i = 0; i < NUM_CONTROLLERS; ++i)
  {
    controllers[i].Poll();
  }
}
