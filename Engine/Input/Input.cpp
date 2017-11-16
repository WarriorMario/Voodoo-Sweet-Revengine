#include "Input.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"
#include "Matrix.h"
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
  switch(code)
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
        return controllers[device_ID].ButtonIsPressed((size_t)code);
      }
      else
      {
        return kbd.KeyIsPressed((unsigned char)code);
      }
    }
  }
}

Vec2 Input::GetAxis(AxisCode code, int device_ID)
{
  switch(code)
  {
    case AxisCode::LEFT:
    {
      return controllers[device_ID].LeftStick();
    }
    case AxisCode::RIGHT:
    {
      return controllers[device_ID].RightStick();
    }
    case AxisCode::TRIGGER:
    {
      return controllers[device_ID].Trigger();
    }
  }
}

Vec2 Input::MousePos()
{
  return Vec2((float)mouse.GetPosX(),(float)mouse.GetPosY());
}

void Input::Poll()
{
  for(int i = 0; i < NUM_CONTROLLERS; ++i)
  {
    controllers[i].Poll();
  }
}
