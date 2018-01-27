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
	b2Vec2 direction = b2Vec2(0.0f, 0.0f);
	if (input.KeyIsPressed(VK_BACK) || input.KeyIsPressed(VK_SPACE))
		return new GettingWater(direction);

  if(input.KeyIsPressed(VK_RIGHT))
  {
    direction.x += Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
    direction.x -= Owner().speed;
    Owner().SetFlipped(false);
  }
  if (input.KeyIsPressed(VK_UP))
	  direction.y += Owner().speed;

  if (input.KeyIsPressed(VK_DOWN))
	  direction.y -= Owner().speed;
	if(direction.x != 0.f && direction.y != 0.f)
  {
	 if ((input.KeyIsPressed(VK_LEFT) || input.KeyIsPressed(VK_RIGHT)) && input.KeyIsPressed(VK_UP))
			return new MoveUpSideState(direction);
	 else
	 if ((input.KeyIsPressed(VK_LEFT) || input.KeyIsPressed(VK_RIGHT)) && input.KeyIsPressed(VK_DOWN))
			return new MoveDownSideState(direction);
	 else
     if(input.KeyIsPressed(VK_LEFT)|| input.KeyIsPressed(VK_RIGHT))
			return new MoveSideState(direction);
	 else
	 if (input.KeyIsPressed(VK_UP) || input.KeyIsPressed(VK_DOWN))
		 return new MoveUpDownState(direction);
  }
  return nullptr;
}

// ****************************************************************************
void MoveSideState::OnEnter(Player& player)
{
	Base::OnEnter(player);

  Owner().SetSprite(Player::MoveSide);

}
void MoveSideState::OnExit()
{

}

MoveSideState::State MoveSideState::Update()
{
	direction.Normalize();
	Owner().x += direction.x * 5.f;

	return nullptr;
}
MoveSideState::State MoveSideState::Input(Keyboard& input)
{

  direction = b2Vec2(0.0f, 0.0f);
  if(input.KeyIsPressed(VK_RIGHT))
  {
	  if (input.KeyIsPressed(VK_UP))
		  return new MoveUpSideState(direction);
	  if (input.KeyIsPressed(VK_DOWN))
		  return new MoveDownSideState(direction);

    direction += Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
	  if (input.KeyIsPressed(VK_UP))
		  return new MoveUpSideState(direction);
	  if (input.KeyIsPressed(VK_DOWN))
		  return new MoveDownSideState(direction);

    direction -= Owner().speed;
    Owner().SetFlipped(false);
  }

  if(direction == 0.f)
  {
    return new IdleState;
  }


  return nullptr;
}

// ****************************************************************************
void MoveUpDownState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::MoveUp);
}
void MoveUpDownState::OnExit()
{

}

MoveUpDownState::State MoveUpDownState::Update()
{
	Owner().y += direction.y * 5.f;

	return nullptr;
}
MoveUpDownState::State MoveUpDownState::Input(Keyboard& input)
{

	direction = b2Vec2(0.0f, 0.0f);
	if (input.KeyIsPressed(VK_DOWN))
	{
		if (input.KeyIsPressed(VK_LEFT) || input.KeyIsPressed(VK_RIGHT))
			return new MoveDownSideState(direction);
		direction.y -= Owner().speed;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_UP))
	{
		if (input.KeyIsPressed(VK_LEFT) || input.KeyIsPressed(VK_RIGHT))
			return new MoveUpSideState(direction);
		direction.y += Owner().speed;
		Owner().SetFlipped(false);
	}
	if (direction.x == 0.f && direction.y == 0)
	{
		return new IdleState;
	}
	return nullptr;
}

// ****************************************************************************
void MoveUpSideState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::MoveUp);
}
void MoveUpSideState::OnExit()
{

}

MoveUpSideState::State MoveUpSideState::Update()
{
	Owner().y += Owner().speed * 5.f;
	Owner().x += direction.x * 5.f;
	return nullptr;
}
MoveUpSideState::State MoveUpSideState::Input(Keyboard& input)
{

	direction = 0.f;
	if (input.KeyIsPressed(VK_UP) && input.KeyIsPressed(VK_RIGHT))
	{
		direction.x += Owner().speed;
		Owner().SetFlipped(false);
	}
	if (input.KeyIsPressed(VK_UP) && input.KeyIsPressed(VK_LEFT))
	{
		direction.x -= Owner().speed;
		Owner().SetFlipped(true);
	}
	if (direction == 0.f)
	{
		return new IdleState;
	}

	return nullptr;
}

// ****************************************************************************
void MoveDownSideState::OnEnter(Player& player)
{
	Base::OnEnter(player);

	Owner().SetSprite(Player::MoveUp);
}
void MoveDownSideState::OnExit()
{

}

MoveDownSideState::State MoveDownSideState::Update()
{
	Owner().y -= Owner().speed * 5.f;
	Owner().x += direction.x * 5.f;
	return nullptr;
}
MoveDownSideState::State MoveDownSideState::Input(Keyboard& input)
{
	direction = 0.f;
	if (input.KeyIsPressed(VK_DOWN) && input.KeyIsPressed(VK_RIGHT))
	{
		direction += Owner().speed;
		Owner().SetFlipped(true);
	}
	if (input.KeyIsPressed(VK_DOWN) && input.KeyIsPressed(VK_LEFT))
	{
		direction -= Owner().speed;
		Owner().SetFlipped(true);
	}
	if (direction == 0.f)
	{
		return new IdleState;
	}
	return nullptr;
}

// ****************************************************************************

// ****************************************************************************
void GettingWater::OnEnter(Player& player)
{
	Base::OnEnter(player);
	gettingWaterAmount = 10.f;
	releaseWaterAmount = -20.f;
	waterAdding = 0;

}
void GettingWater::OnExit()
{

}

GettingWater::State GettingWater::Update()
{
	if (waterAdding > -100.f && waterAdding < 100.f)
		Owner().waterPercentage += waterAdding;
	if (Owner().waterPercentage > 100)
		Owner().waterPercentage = 100;
	if (Owner().waterPercentage < 0)
		Owner().waterPercentage = 0;
	if (Owner().waterPercentage > 0 && Owner().waterPercentage < 100) {
		Owner().speed = -((Owner().waterPercentage - 100.f) / 100);
	}

	return nullptr;
}
GettingWater::State GettingWater::Input(Keyboard& input)
{
	waterAdding = 0;
	if (!input.KeyIsDown(VK_SPACE) && !input.KeyIsDown(VK_BACK))
		return new IdleState;

	if (input.KeyIsDown(VK_SPACE))
		waterAdding = gettingWaterAmount;


	if (input.KeyIsDown(VK_BACK))
		waterAdding = releaseWaterAmount;

		return new IdleState;
	return nullptr;
}


// ****************************************************************************
Player::Player()
  :
  movement(*this, new IdleState),
  sprites{"Images/idle1.png", "Images/move1.png", "Images/MoveUp.png", "Images/MoveUpSide.png","Images/MoveDownSide.png"},
  flip_sprite(false),
	font(Font("Fonts/times.ttf"))
{
  width = 100.f;
  height = 100.f;
  x = 1920.f * 0.5f;
  y = 1080.f * 0.5f;
  speed = 1.0f;
}
