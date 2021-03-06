#include "Player.h"
#include "Input\Input.h"
#include "Physics\Physics.h"
#include "TileGrid.h"
#include "Tile.h"
#include "Utility\Serialization.h"
#include "Utility\Json\json.h"

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

float Player::MinSpeed;
float Player::BaseScale;
float Player::ScaleAmplifier;
float Player::BaseSpeed;
float Player::GodBaseSpeed;
float Player::water_added_to_goal = 0.0f;
float Player::water_needed_to_win = 0.0f;

float GettingWater::ConsumeWaterSpeed;
float GettingWater::ReleaseWaterSpeed;
float GettingWater::ChunkTime;

Player::Player(Physics& simulation, TileGrid& grid, int id)
  :
  movement(*this, new IdleState),
  collision_box_scale_base(b2Vec2(0.0f)),
  collision_box_scale_cur(b2Vec2(0.0f)),
  flip_sprite(false),
  physics_body(simulation.CreateBody(Vec2(40, 30), "Square")),
  grid(grid),
  player_id(id),
  graphics{0.1f, 0.1f, 0.1f, 0.1f, 0.1f }
{
	// load in the variables from the json file
	LoadVariables();

	graphics[(int)Sprite::Idle].AddLayer(PLUS_WATER_LAYER_0, 1);
	graphics[(int)Sprite::Idle].AddLayer(PLUS_WATER_LAYER_1, 1);
	graphics[(int)Sprite::Idle].AddLayer(PLUS_WATER_LAYER_2, 1);
	graphics[(int)Sprite::Idle].AddLayer(PLUS_WATER_LAYER_3, 1);

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


	graphics[(int)Sprite::Idle].ScaleLayer(0, BaseScale);
	graphics[(int)Sprite::Idle].ScaleLayer(1, BaseScale);
	graphics[(int)Sprite::Idle].ScaleLayer(2, BaseScale);
	graphics[(int)Sprite::Idle].ScaleLayer(3, BaseScale);
	graphics[(int)Sprite::Move].ScaleLayer(0, BaseScale);
	graphics[(int)Sprite::Move].ScaleLayer(1, BaseScale);
	graphics[(int)Sprite::Move].ScaleLayer(2, BaseScale);
	graphics[(int)Sprite::Move].ScaleLayer(3, BaseScale);
	graphics[(int)Sprite::AddingWater].ScaleLayer(0, BaseScale);
	graphics[(int)Sprite::AddingWater].ScaleLayer(1, BaseScale);
	graphics[(int)Sprite::AddingWater].ScaleLayer(2, BaseScale);
	graphics[(int)Sprite::AddingWater].ScaleLayer(3, BaseScale);
	graphics[(int)Sprite::DumpingWater].ScaleLayer(0, BaseScale);
	graphics[(int)Sprite::DumpingWater].ScaleLayer(1, BaseScale);
	graphics[(int)Sprite::DumpingWater].ScaleLayer(2, BaseScale);
	graphics[(int)Sprite::DumpingWater].ScaleLayer(3, BaseScale);

	width = 32.0f;
	height = 32.0f;
	x = 400.0f;
	y = 300.0f;
	physics_body.body->SetUserData(this);
	water_forced_out = false;
	speed = BaseSpeed;
}

Player::Player(Physics& simulation, TileGrid& grid)
  :
  movement(*this, new IdleState),
  collision_box_scale_base(b2Vec2(0.0f)),
  collision_box_scale_cur(b2Vec2(0.0f)),
  flip_sprite(false),
  physics_body(simulation.CreateBody(Vec2(40, 30), "Square")),
  grid(grid),
  graphics{0.1f, 0.1f, 0.1f, 0.1f, 0.1f}
{
  // load in the variables from the json file
  LoadVariables();

  width = 32.0f;
  height = 32.0f;
  x = 400.0f;
  y = 300.0f;
  physics_body.body->SetUserData(this);
  water_forced_out = false;
  speed = BaseSpeed;
}
void Player::Update(float dt)
{
	total_time += dt;


	movement.Update(dt);
	graphics[(int)curr_sprite].Update(dt);

	physics_body.body->GetWorld()->ClearForces();
	physics_body.body->SetTransform(Vec2(x, y) / PHYSICS_SCALE, 0);
}

bool Player::LoadVariables()
{
	StringRef loadName = VARIABLES_TO_LOAD;
	Serializer ser("");
	if (ser.Deserialize(loadName.data()) == ErrorCodes::FAILURE)
	{
		// failed to load the json file
		return false;
	}
	ser.Get("basescale", BaseScale);
	ser.Get("scaleamplifier", ScaleAmplifier);
	ser.Get("basespeed", BaseSpeed);
	ser.Get("minspeed", MinSpeed);
	ser.Get("godbasespeed", GodBaseSpeed);
	ser.Get("waterneededtowin", water_needed_to_win);

	ser.Get("consumewaterspeed", GettingWater::ConsumeWaterSpeed);
	ser.Get("releasewaterspeed", GettingWater::ReleaseWaterSpeed);
	ser.Get("chunktime", GettingWater::ChunkTime);

	ser.Get("collisionboxscalex", collision_box_scale_base.x);
	ser.Get("collisionboxscaley", collision_box_scale_base.y);
	collision_box_scale_cur = collision_box_scale_base;
}

void Player::Input(::Input & input)
{
	movement.Input(input);
}

bool Player::IsStuck()
{
	bool res = false;

	// outer corners
	res |= grid.IsPassable((x + collision_box_scale_cur.x) / Tile::SIZE, (y + collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x + collision_box_scale_cur.x) / Tile::SIZE, (y - collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - collision_box_scale_cur.x) / Tile::SIZE, (y + collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - collision_box_scale_cur.x) / Tile::SIZE, (y - collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;

	// centre points
	res |= grid.IsPassable((x + collision_box_scale_cur.x) / Tile::SIZE, y / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable((x - collision_box_scale_cur.x) / Tile::SIZE, y / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable(x / Tile::SIZE, (y + collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;
	res |= grid.IsPassable(x / Tile::SIZE, (y - collision_box_scale_cur.y) / Tile::SIZE, is_god) == false;

	return res;
}
bool Player::CanDrink()
{
	return grid.DrinkingArea(x / Tile::SIZE, y / Tile::SIZE);
}

bool Player::CanDepositWater()
{
	return grid.DepositingArea(x / Tile::SIZE, y / Tile::SIZE);
}

void Player::LoseWater()
{
	if (waterPercentage > 0)
	{
		water_forced_out = true;
		water_goes_in = false;
	}
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
constexpr float delay = 0.6f;
IdleState::State IdleState::Update(float dt)
{
  if(Owner().hitting == true)
  {
    static float last_time = 0.0f;
    if(last_time + delay < Timer::Time())
    {
      smash.Play();
      last_time = Timer::Time();
    }
    Owner().SetSprite(Player::Smash);
  }
  else
  {
    Owner().SetSprite(Player::Idle);
  }
  return nullptr;
}
IdleState::State IdleState::Input(::Input& input)
{
	b2Vec2 dir = b2Vec2(0.0f, 0.0f);
	if (Owner().is_god == false)
	{
		if ((input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id) && Owner().CanDrink()))
		{
			Owner().water_goes_in = true;
			return new GettingWater();
		}
		else if ((input.IsDown(ButtonCode::GAMEPAD_B, Owner().player_id) ||
			Owner().water_forced_out == true) && Owner().waterPercentage > 0)
		{
			Owner().water_goes_in = false;
			return new GettingWater();
		}
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).x > 0)
	{
		dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
		Owner().SetFlipped(true);
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
	{
		dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
		Owner().SetFlipped(false);
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
	{
		dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
		Owner().SetFlipped(true);
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
	{
		dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
		Owner().SetFlipped(false);
	}
	if (dir.x != 0.f || dir.y != 0.0f)
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
  if(Owner().hitting == true)
  {
    Owner().SetSprite(Player::Smash);
  }
  else
  {
    Owner().SetSprite(Player::Move);
  }
	if (Owner().water_forced_out == true && Owner().waterPercentage > 0)
	{
		return new GettingWater;
	}
	dir.Normalize();
	if (abs(dir.x) > abs(dir.y))
	{
		Owner().x += dir.x * Owner().speed;
		if (Owner().IsStuck())
		{
			Owner().x -= dir.x * Owner().speed;
		}
		return nullptr;
	}
	Owner().y += dir.y * Owner().speed;
	if (Owner().IsStuck())
	{
		Owner().y -= dir.y * Owner().speed;
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
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).x > 0)
	{
		dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
	{
		dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x * Owner().speed;
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
	{
		dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
	}
	if (input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
	{
		dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y * Owner().speed;
	}
	if (dir.x == 0.f && dir.y == 0)
	{
		return new IdleState;
	}
	//else if (input.KeyIsPressed(VK_SHIFT))
	//{
	//	return new RunState(dir);
	//}
	if (dir.x > 0)
	{
		Owner().SetFlipped(false);
	}
	else
	{
		Owner().SetFlipped(true);
	}

  return nullptr;
}

// ****************************************************************************
void GettingWater::OnEnter(Player& player)
{
	Base::OnEnter(player);
	Owner().SetSprite(Owner().water_goes_in ? Player::Sprite::AddingWater : Player::Sprite::DumpingWater);
	waterAdding = 0;
	last_update = 0;
	puke_sound.Stop();
	slurp_sound.Stop();
	Owner().water_goes_in ? slurp_sound.Play() : puke_sound.Play();
}
void GettingWater::OnExit()
{

}

GettingWater::State GettingWater::Update(float dt)
{
	if (last_update > 0)
	{
		last_update -= dt;
		return nullptr;
	}
	if (Owner().water_forced_out == true)
	{
		Owner().water_forced_out = (Owner().waterPercentage > 0);
	}
	//checks if the waterAdding is the right amount and adds that amount to the water percentage
	if (waterAdding == ConsumeWaterSpeed)
	{
		Owner().waterPercentage += waterAdding;
	}
	else if (waterAdding == ReleaseWaterSpeed)
	{
		Owner().waterPercentage += waterAdding;
		if (Owner().CanDepositWater())
		{
			Owner().water_added_to_goal -= waterAdding;
		}
	}
	//This makes sure it stays within the 100% range
	if (Owner().waterPercentage > 100)
		Owner().waterPercentage = 100;
	//This makes sure it doesn't go lower than 0
	if (Owner().waterPercentage < 0)
		Owner().waterPercentage = 0;
	//Makes sure the speed is lower when the waterpercentage is higher 
	if (Owner().waterPercentage > 0 && Owner().waterPercentage < 100)
	{
		Owner().speed = Owner().BaseSpeed - (Owner().waterPercentage / 100.0f)*(Owner().BaseSpeed - Owner().MinSpeed);
	}

	float scale = Owner().ScaleAmplifier *(Owner().waterPercentage / 100.0f) + Owner().BaseScale;
	float y_offset = (scale - Owner().BaseScale)* 8.0f;
	Owner().graphics[(int)Player::Sprite::Idle].ScaleLayer(0, scale);
	Owner().graphics[(int)Player::Sprite::Idle].ScaleLayer(1, scale);
	Owner().graphics[(int)Player::Sprite::Idle].ScaleLayer(2, scale);
	Owner().graphics[(int)Player::Sprite::Move].ScaleLayer(0, scale);
	Owner().graphics[(int)Player::Sprite::Move].ScaleLayer(1, scale);
	Owner().graphics[(int)Player::Sprite::Move].ScaleLayer(2, scale);
	Owner().graphics[(int)Player::Sprite::AddingWater].ScaleLayer(0, scale);
	Owner().graphics[(int)Player::Sprite::AddingWater].ScaleLayer(1, scale);
	Owner().graphics[(int)Player::Sprite::AddingWater].ScaleLayer(2, scale);
	Owner().graphics[(int)Player::Sprite::DumpingWater].ScaleLayer(0, scale);
	Owner().graphics[(int)Player::Sprite::DumpingWater].ScaleLayer(1, scale);
	Owner().graphics[(int)Player::Sprite::DumpingWater].ScaleLayer(2, scale);
	Owner().graphics[(int)Player::Sprite::Idle].OffsetLayer(0, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Idle].OffsetLayer(1, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Idle].OffsetLayer(2, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Idle].OffsetLayer(3, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Move].OffsetLayer(0, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Move].OffsetLayer(1, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Move].OffsetLayer(2, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::Move].OffsetLayer(3, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::AddingWater].OffsetLayer(0, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::AddingWater].OffsetLayer(1, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::AddingWater].OffsetLayer(2, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::AddingWater].OffsetLayer(3, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::DumpingWater].OffsetLayer(0, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::DumpingWater].OffsetLayer(1, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::DumpingWater].OffsetLayer(2, Vec2(0, y_offset));
	Owner().graphics[(int)Player::Sprite::DumpingWater].OffsetLayer(3, Vec2(0, y_offset));
	//Owner().graphics[(int)Player::Sprite::Move].OffsetLayer(1, Vec2(0, 0 * t));

	if (waterAdding < 0 && Owner().water_forced_out == true)
	{
		last_update = ChunkTime;
	}
	return nullptr;
}
GettingWater::State GettingWater::Input(::Input& input)
{
	//If it's not consuming or releasing it sends it back to the Idle state
	if (!input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id) &&
		!input.IsDown(ButtonCode::GAMEPAD_B, Owner().player_id) &&
		Owner().water_forced_out == false)
	{
		return new IdleState;
	}
	// Checks if you're consuming and if so it changes the 
	if (input.IsDown(ButtonCode::GAMEPAD_A, Owner().player_id) && Owner().CanDrink() && Owner().water_forced_out == false)
	{
		waterAdding = ConsumeWaterSpeed;
	}
	else if ((input.IsDown(ButtonCode::GAMEPAD_B, Owner().player_id) || Owner().water_forced_out == true) && Owner().waterPercentage > 0.0f)
	{
		waterAdding = ReleaseWaterSpeed;
	}
	else
	{
		return new IdleState;
	}

	return nullptr;
}
