#include "Player.h"
#include "Input\Input.h"
#include "Physics\Physics.h"
#include "TileGrid.h"
#include "Tile.h"

// ****************************************************************************
#define MOVE_LAYER_0 "Images/Kid1_Anim/Layer0/0_Kid1"
#define MOVE_LAYER_1 "Images/Kid1_Anim/Layer1/1_Kid1"
#define MOVE_LAYER_2 "Images/Kid1_Anim/Layer2/2_Kid1"
#define MOVE_LAYER_3 "Images/Kid1_Anim/Layer3/3_Kid1"

#define PLUS_WATER_LAYER_0 "Images/Kid1_Anim/Layer0/0_Kid1W"
#define PLUS_WATER_LAYER_1 "Images/Kid1_Anim/Layer1/1_Kid1W"
#define PLUS_WATER_LAYER_2 "Images/Kid1_Anim/Layer2/2_Kid1W"
#define PLUS_WATER_LAYER_3 "Images/Kid1_Anim/Layer3/3_Kid1W"

#define MIN_WATER_LAYER_0 "Images/Kid1_Anim/Layer0/0_Kid1Hit"
#define MIN_WATER_LAYER_1 "Images/Kid1_Anim/Layer1/1_Kid1Hit"
#define MIN_WATER_LAYER_2 "Images/Kid1_Anim/Layer2/2_Kid1Hit"
#define MIN_WATER_LAYER_3 "Images/Kid1_Anim/Layer3/3_Kid1Hit"

Player::Player(Physics& simulation, TileGrid& grid, int id)
	:
	movement(*this, new IdleState),
	flip_sprite(false),
	physics_body(simulation.CreateBody(Vec2(40, 30), "Square")),
	grid(grid),
	player_id(id),
	graphics{ 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f }
{
	graphics[(int)Sprite::Idle].AddLayer(MOVE_LAYER_0, 3);
	graphics[(int)Sprite::Idle].AddLayer(MOVE_LAYER_1, 3);
	graphics[(int)Sprite::Idle].AddLayer(MOVE_LAYER_2, 3);
	graphics[(int)Sprite::Idle].AddLayer(MOVE_LAYER_3, 3);

	graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_0, 3);
	graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_1, 3);
	graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_2, 3);
	graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_3, 3);

	graphics[(int)Sprite::AddingWater].AddLayer(PLUS_WATER_LAYER_0, 19);
	graphics[(int)Sprite::AddingWater].AddLayer(PLUS_WATER_LAYER_1, 19);
	graphics[(int)Sprite::AddingWater].AddLayer(PLUS_WATER_LAYER_2, 19);
	graphics[(int)Sprite::AddingWater].AddLayer(PLUS_WATER_LAYER_3, 19);

	graphics[(int)Sprite::DumpingWater].AddLayer(MIN_WATER_LAYER_0, 1);
	graphics[(int)Sprite::DumpingWater].AddLayer(MIN_WATER_LAYER_1, 1);
	graphics[(int)Sprite::DumpingWater].AddLayer(MIN_WATER_LAYER_2, 1);
	graphics[(int)Sprite::DumpingWater].AddLayer(MIN_WATER_LAYER_3, 1);


  graphics[(int)Sprite::Idle].ScaleLayer(0, 1.0f);
  graphics[(int)Sprite::Idle].ScaleLayer(1, 1.0f);
  graphics[(int)Sprite::Idle].ScaleLayer(2, 1.0f);
  graphics[(int)Sprite::Idle].ScaleLayer(3, 1.0f);
  graphics[(int)Sprite::Move].ScaleLayer(0, 1.0f);
  graphics[(int)Sprite::Move].ScaleLayer(1, 1.0f);
  graphics[(int)Sprite::Move].ScaleLayer(2, 1.0f);
  graphics[(int)Sprite::Move].ScaleLayer(3, 1.0f);
  graphics[(int)Sprite::AddingWater].ScaleLayer(0, 1.0f);
  graphics[(int)Sprite::AddingWater].ScaleLayer(1, 1.0f);
  graphics[(int)Sprite::AddingWater].ScaleLayer(2, 1.0f);
  graphics[(int)Sprite::AddingWater].ScaleLayer(3, 1.0f);
  graphics[(int)Sprite::DumpingWater].ScaleLayer(0, 1.0f);
  graphics[(int)Sprite::DumpingWater].ScaleLayer(1, 1.0f);
  graphics[(int)Sprite::DumpingWater].ScaleLayer(2, 1.0f);
  graphics[(int)Sprite::DumpingWater].ScaleLayer(3, 1.0f);

	width = 32.0f;
	height = 32.0f;
	x = 400.0f;
	y = 300.0f;
	physics_body.body->SetUserData(this);
	dead = false;
}

void Player::Update(float dt)
{
	if (dead)
	{
		return;
	}

	total_time += dt;


	float t = sinf(total_time * 2.0f) / 2.0f + 0.5f;
	float scale = t + 1.5f;
	graphics[(int)Sprite::Idle].ScaleLayer(0, scale);
	graphics[(int)Sprite::Idle].ScaleLayer(1, scale);
	graphics[(int)Sprite::Idle].ScaleLayer(2, scale);
	graphics[(int)Sprite::Idle].OffsetLayer(1, Vec2(0, 0 * t));
	graphics[(int)Sprite::Move].ScaleLayer(0, scale);
	graphics[(int)Sprite::Move].ScaleLayer(1, scale);
	graphics[(int)Sprite::Move].ScaleLayer(2, scale);
	graphics[(int)Sprite::Move].OffsetLayer(1, Vec2(0, 0 * t));

	movement.Update(dt);
	graphics[(int)curr_sprite].Update(dt);

	physics_body.body->GetWorld()->ClearForces();
	physics_body.body->SetTransform(Vec2(x, y) / PHYSICS_SCALE, 0);
}

void Player::Input(::Input & input)
{
	movement.Input(input);
}

bool Player::IsStuck()
{
	bool res = false;
	Vec2 offset = Vec2(width / 2, height / 2);

	// outer corners
	res |= grid.IsPassable((x + offset.x) / Tile::SIZE, (y + offset.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x + offset.x) / Tile::SIZE, (y - offset.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - offset.x) / Tile::SIZE, (y + offset.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - offset.x) / Tile::SIZE, (y - offset.y) / Tile::SIZE, is_god) == false;

	// centre points
	res |= grid.IsPassable((x + offset.x) / Tile::SIZE, y / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - offset.x) / Tile::SIZE, y / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable(x / Tile::SIZE, (y + offset.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable(x / Tile::SIZE, (y - offset.y) / Tile::SIZE, is_god) == false;

	return res;
}

bool Player::CanDrink()
{
	return grid.DrinkingArea(x / Tile::SIZE, y / Tile::SIZE);
}

void Player::LoseWater()
{
	// Not implemented yet
	dead = true;
}

// ****************************************************************************
void IdleState::OnEnter(Player& player)
{
  Base::OnEnter(player);

  Owner().SetSprite(Player::Idle);
}
void IdleState::OnExit()
{

}

IdleState::State IdleState::Update(float dt)
{
  return nullptr;
}
IdleState::State IdleState::Input(::Input& input)
{
  b2Vec2 dir = b2Vec2(0.0f, 0.0f);
  if(Owner().is_god == false)
  {
    if((input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id) && Owner().CanDrink()))
    {
      Owner().water_goes_in = true;
      return new GettingWater();
    }
    else if(input.IsDown(ButtonCode::GAMEPAD_B, Owner().player_id))
    {
      Owner().water_goes_in = false;
      return new GettingWater();
    }
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x > 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
    Owner().SetFlipped(false);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
    Owner().SetFlipped(false);
  }
  if(dir.x != 0.f || dir.y != 0.0f)
  {
    //if (input.KeyIsPressed(VK_SHIFT))
    //{
    //	return new RunState(dir);
    //}
    //else
    //{
    return new MoveState(dir);
    //}
  }
  //if (input.KeyIsPressed(VK_SPACE))
  //{
  //	return new JumpState;
  //}

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

MoveState::State MoveState::Update(float dt)
{
  dir.Normalize();
  Owner().x += dir.x * 5.f;
  Owner().y += dir.y * 5.f;
  if(Owner().IsStuck())
  {
    Owner().x -= dir.x * 5.f;
    Owner().y -= dir.y * 5.f;
  }

  return nullptr;
}
MoveState::State MoveState::Input(::Input& input)
{
  //if (input.KeyIsPressed(VK_SPACE))
  //{
  //	return new JumpState;
  //}

  dir = b2Vec2(0.0f, 0.0f);
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x > 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
    Owner().SetFlipped(false);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
    Owner().SetFlipped(false);
  }
  if(dir.x == 0.f && dir.y == 0)
  {
    return new IdleState;
  }
  //else if (input.KeyIsPressed(VK_SHIFT))
  //{
  //	return new RunState(dir);
  //}

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

RunState::State RunState::Update(float dt)
{
  direction.Normalize();
  Owner().x += direction.x * 10.f;
  Owner().y += direction.y * 10.f;

  return nullptr;
}
RunState::State RunState::Input(::Input& input)
{
  //if (input.KeyIsPressed(VK_SPACE))
  //{
  //	return new JumpState;
  //}
  //
  //dir = b2Vec2(0.0f, 0.0f);
  //if (input.KeyIsPressed(VK_RIGHT))
  //{
  //	dir.x += 1.f;
  //	Owner().SetFlipped(true);
  //}
  //if (input.KeyIsPressed(VK_LEFT))
  //{
  //	dir.x -= 1.f;
  //	Owner().SetFlipped(false);
  //}
  //if (input.KeyIsPressed(VK_UP))
  //{
  //	dir.y += 1.f;
  //	Owner().SetFlipped(true);
  //}
  //if (input.KeyIsPressed(VK_DOWN))
  //{
  //	dir.y -= 1.f;
  //	Owner().SetFlipped(false);
  //}
  //if (dir.x == 0.0f && dir.y == 0.0f)
  //{
  //	return new IdleState;
  //}
  //else if (!input.KeyIsPressed(VK_SHIFT))
  //{
  //	return new MoveState(dir);
  //}

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

JumpState::State JumpState::Update(float dt)
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
JumpState::State JumpState::Input(::Input& input)
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
void GettingWater::OnEnter(Player& player)
{
	Base::OnEnter(player);
  Owner().SetSprite(Owner().water_goes_in ? Player::Sprite::AddingWater : Player::Sprite::DumpingWater);
	//Sets the amount of water that it can consume/release simultaneously
	consumingWaterAmount = 10.f;
	releaseWaterAmount = -20.f;
	waterAdding = 0;
}
void GettingWater::OnExit()
{

}

GettingWater::State GettingWater::Update(float dt)
{
	//checks if the waterAdding is the right amount and adds that amount to the water percentage
	if (waterAdding == consumingWaterAmount || waterAdding == releaseWaterAmount)
		Owner().waterPercentage += waterAdding;
	//This makes sure it stays within the 100% range
	if (Owner().waterPercentage > 100)
		Owner().waterPercentage = 100;
	//This makes sure it doesn't go lower than 0
	if (Owner().waterPercentage < 0)
		Owner().waterPercentage = 0;
	//Makes sure the speed is lower when the waterpercentage is higher 
	if (Owner().waterPercentage > 0 && Owner().waterPercentage < 100) {
		Owner().speed = -((Owner().waterPercentage - 100.f) / 100.f) + 1.f;
	}

	return nullptr;
}
GettingWater::State GettingWater::Input(::Input& input)
{
	//If it's not consuming or releasing it sends it back to the Idle state
	if (!input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id) && !input.IsDown(ButtonCode::GAMEPAD_B,Owner().player_id))
		return new IdleState;
	// Checks if you're consuming and if so it changes the 
	if (input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id))
		waterAdding = consumingWaterAmount;


	if (input.IsDown(ButtonCode::GAMEPAD_B, Owner().player_id))
		waterAdding = releaseWaterAmount;

	return nullptr;
}
