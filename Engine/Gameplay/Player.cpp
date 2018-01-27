#include "Player.h"

// ****************************************************************************
void IdleState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::Idle);
}
void IdleState::OnExit()
{

}

IdleState::State IdleState::Update()
{
	return nullptr;
}
IdleState::State IdleState::Input(Keyboard& input)
{
	b2Vec2 dir = b2Vec2(0.0f, 0.0f);
	if (input.KeyIsPressed(VK_RIGHT))
	{
		dir.x += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_LEFT))
	{
		dir.x -= 1.f;
		Owner().SetFlipped(false);
	}
	if (input.KeyIsPressed(VK_UP))
	{
		dir.y += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_DOWN))
	{
		dir.y -= 1.f;
		Owner().SetFlipped(false);
	}
	if (dir.x != 0.f || dir.y != 0.0f)
	{
		if (input.KeyIsPressed(VK_SHIFT))
		{
			return new RunState(dir);
		}
		else
		{
			return new MoveState(dir);
		}
	}
	if (input.KeyIsPressed(VK_SPACE))
	{
		return new JumpState;
	}

	return nullptr;
}

// ****************************************************************************
void MoveState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::Move);
}
void MoveState::OnExit()
{

}

MoveState::State MoveState::Update()
{
	dir.Normalize();
	Owner().x += dir.x * 5.f;
	Owner().y += dir.y * 5.f;

	return nullptr;
}
MoveState::State MoveState::Input(Keyboard& input)
{
	if (input.KeyIsPressed(VK_SPACE))
	{
		return new JumpState;
	}

	dir = b2Vec2(0.0f, 0.0f);
	if (input.KeyIsPressed(VK_RIGHT))
	{
		dir.x += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_LEFT))
	{
		dir.x -= 1.f;
		Owner().SetFlipped(false);
	}
	if (input.KeyIsPressed(VK_UP))
	{
		dir.y += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_DOWN))
	{
		dir.y -= 1.f;
		Owner().SetFlipped(false);
	}
	if (dir.x == 0.f && dir.y)
	{
		return new IdleState;
	}
	else if (input.KeyIsPressed(VK_SHIFT))
	{
		return new RunState(dir);
	}

	return nullptr;
}

// ****************************************************************************
void RunState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::Run);
}
void RunState::OnExit()
{

}

RunState::State RunState::Update()
{
	dir.Normalize();
	Owner().x += dir.x * 10.f;
	Owner().y += dir.y * 10.f;

	return nullptr;
}
RunState::State RunState::Input(Keyboard& input)
{
	if (input.KeyIsPressed(VK_SPACE))
	{
		return new JumpState;
	}

	dir = b2Vec2(0.0f, 0.0f);
	if (input.KeyIsPressed(VK_RIGHT))
	{
		dir.x += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_LEFT))
	{
		dir.x -= 1.f;
		Owner().SetFlipped(false);
	}
	if (input.KeyIsPressed(VK_UP))
	{
		dir.y += 1.f;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_DOWN))
	{
		dir.y -= 1.f;
		Owner().SetFlipped(false);
	}
	if (dir.x == 0.0f && dir.y == 0.0f)
	{
		return new IdleState;
	}
	else if (!input.KeyIsPressed(VK_SHIFT))
	{
		return new MoveState(dir);
	}

	return nullptr;
}

// ****************************************************************************
void JumpState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	base_y = player.y;

	fart.Play();

	//Owner().SetSprite(Player::Jump);
}
void JumpState::OnExit()
{
	Owner().y = base_y;
}

JumpState::State JumpState::Update()
{
	//jump_y += velocity * 0.015f;
	//
	//velocity -= 25.f; // fake drag
	//velocity = Max(velocity, -500.f); // fake terminal velocity
	//
	//if (jump_y < 0.f)
	//{
		return new IdleState;
	//}
	//Owner().y = base_y - jump_y;
	//
	//Owner().x += direction * 5.f;
	//
	//return nullptr;
}
JumpState::State JumpState::Input(Keyboard& input)
{
	//direction = 0.f;
	//if (input.KeyIsPressed(VK_RIGHT))
	//{
	//	direction += 1.f;
	//	Owner().SetFlipped(true);
	//}
	//if (input.KeyIsPressed(VK_LEFT))
	//{
	//	direction -= 1.f;
	//	Owner().SetFlipped(false);
	//}
	//
	return nullptr;
}

// ****************************************************************************
Player::Player()
	:
	movement(*this, new IdleState),
	sprites{ "Images/idle.png", "Images/move.png", "Images/run.png", "Images/jump.png" },
	flip_sprite(false),
	font(Font("Fonts/times.ttf"))
{
	width = 128.0f;
	height = 151.0f;
	x = 300.0f;
	y = 300.0f;
}
