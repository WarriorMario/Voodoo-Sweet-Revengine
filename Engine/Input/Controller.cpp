#include "Controller.h"
#include "ButtonCodes.h"
#include "Matrix.h"
void Controller::Poll()
{
  XINPUT_STATE state = {};

  // Simply get the state of the controller from XInput.
  DWORD dwResult = XInputGetState((DWORD)ID, &state);

  if(dwResult == ERROR_SUCCESS)
  {
    controller = state.Gamepad;
  }
  else
  {
    controller = {};
  }
}

bool Controller::ButtonIsPressed(size_t code)
{
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    return controller.bLeftTrigger > trigger_button_threshold;
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    return controller.bRightTrigger > trigger_button_threshold;
  }
  return (code >> 8) & controller.wButtons;
}

Vec2 Controller::LeftStick()
{
  return Vec2(controller.sThumbLX, controller.sThumbLY) / 32768.0f;
}

Vec2 Controller::RightStick()
{
  return Vec2(controller.sThumbRX, controller.sThumbRY) / 32768.0f;
}

Vec2 Controller::Trigger()
{
  return Vec2(controller.bLeftTrigger, controller.bRightTrigger) / 255.f;
}


