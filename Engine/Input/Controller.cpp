#include "Controller.h"
#include "ButtonCodes.h"
#include "Matrix.h"
#include "Utility.h"
#include "ChiliMath.h"

void Controller::Poll()
{
	XINPUT_STATE state = {};

	// Simply get the state of the controller from XInput.
	DWORD dwResult = XInputGetState((DWORD)ID, &state);

	last_controller = controller;
	if (dwResult == ERROR_SUCCESS)
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
	// Last state returns false, this state returns true
	bool last_state;
	// Special case for triggers as buttons
	if (code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
	{
		last_state = (controller.bLeftTrigger > trigger_button_threshold);
	}
	else if (code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
	{
		last_state = (controller.bRightTrigger > trigger_button_threshold);
	}
	last_state = ((code >> 8) & controller.wButtons);
	if (last_state == true)
	{
		return false;
	}
	// Special case for triggers as buttons
	if (code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
	{
		return (controller.bLeftTrigger > trigger_button_threshold) == true;
	}
	else if (code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
	{
		return (controller.bRightTrigger > trigger_button_threshold) == true;
	}
	return ((code >> 8) & controller.wButtons) == true;
}

bool Controller::ButtonIsReleased(size_t code)
{
	// Last state returns true, this state returns false
	bool last_state;
	// Special case for triggers as buttons
	if (code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
	{
		last_state = (controller.bLeftTrigger > trigger_button_threshold);
	}
	else if (code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
	{
		last_state = (controller.bRightTrigger > trigger_button_threshold);
	}
	last_state = ((code >> 8) & controller.wButtons);
	if (last_state == false)
	{
		return false;
	}
	// Special case for triggers as buttons
	if (code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
	{
		return (controller.bLeftTrigger > trigger_button_threshold) == false;
	}
	else if (code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
	{
		return (controller.bRightTrigger > trigger_button_threshold) == false;
	}
	return ((code >> 8) & controller.wButtons) == false;
}

bool Controller::ButtonIsDown(size_t code)
{
	// Special case for triggers as buttons
	if (code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
	{
		return controller.bLeftTrigger > trigger_button_threshold;
	}
	else if (code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
	{
		return controller.bRightTrigger > trigger_button_threshold;
	}
	return (code >> 8) & controller.wButtons;
}

Vec2 Controller::LeftStick()
{
	Vec2 res = Vec2(controller.sThumbLX, controller.sThumbLY) / 32768.0f;
	ApplyDeadzone(res);
	return res;
}

Vec2 Controller::RightStick()
{
	Vec2 res = Vec2(controller.sThumbRX, controller.sThumbRY) / 32768.0f;
	ApplyDeadzone(res);
	return res;
}

Vec2 Controller::Trigger()
{
	Vec2 res = Vec2(controller.bLeftTrigger, controller.bRightTrigger) / 255.f;
	ApplyDeadzone(res);
	return res;
}

void Controller::ApplyDeadzone(Vec2 & input)
{
	// clamp to deadzone
	input.x = abs(input.x) < DEAD_ZONE_THRESHOLD ? 0.0f : input.x;
	input.y = abs(input.y) < DEAD_ZONE_THRESHOLD ? 0.0f : input.y;
	input = input / (1.0f - DEAD_ZONE_THRESHOLD);
}


