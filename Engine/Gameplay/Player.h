#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Utility\DrawUtils.h"
#include "Utility\SpriteAnimation.h"
#include "Keyboard.h"
#include "Physics\Body.h"

// Should be moved somewhere else
static const size_t NUM_PLAYERS = 2;

class Physics;
class TileGrid;

// ****************************************************************************
class Player
{
public:
	enum Sprite
	{
		Idle,
		Move,
		AddingWater,
		DumpingWater,
    Smash,
		NumSprites,
	};

public:
	Player(Physics& simulation, TileGrid& grid, int id);
  Player(Physics& simulation, TileGrid& grid);

	virtual void Update(float dt);
	void Input(Input& input);
	void Draw()
	{
		b2Vec2 pos = b2Vec2(x, y);
		b2Vec2 size = b2Vec2(width, height);
		graphics[(int)curr_sprite].Draw(pos, size, flip_sprite, true);
	}
	bool LoadVariables();
	void SetSprite(Sprite sprite)
	{
		curr_sprite = sprite;
	}
	void SetFlipped(bool flipped)
	{
		flip_sprite = flipped;
	}

  bool IsStuck();
  bool CanDrink();
  bool CanDepositWater();
  void LoseWater();

public:
	float x, y;
	float width, height;
	int player_id;
	bool is_god;
	bool water_forced_out;
  bool water_goes_in;
	Body physics_body;
	float waterPercentage = 0;
	float speed;
	float total_time;
	static float MinSpeed;
	static float BaseScale;
	static float ScaleAmplifier;
	static float BaseSpeed;
  static float GodBaseSpeed;
  float water_added_to_goal;
  bool hitting = false;
	LayeredAnimation graphics[NumSprites];
	Sprite curr_sprite;
  Sprite last_sprite;
private:
	TileGrid & grid;
	static constexpr char VARIABLES_TO_LOAD[] = "Variables/Variables.json";
	StateMachine<Player> movement;
	b2Vec2 collision_box_scale_base;
	b2Vec2 collision_box_scale_cur;

	bool flip_sprite;
};

// ****************************************************************************
// has the player stand idle
class IdleState : public IState<Player>
{
public:
	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;
};

// ****************************************************************************
// it. moves.
class MoveState : public IState<Player>
{
public:
	MoveState(b2Vec2 direction)
		:
		dir(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;

private:
	b2Vec2 dir;
};

// ****************************************************************************
// faster than the move state
class RunState : public IState<Player>
{
public:
	RunState(b2Vec2 direction)
		:
		direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;

private:
	Vec2 direction;
};

// ****************************************************************************
// pretty much obsolete, but does a nice fart sound nonetheless
class JumpState : public IState<Player>
{
public:
	JumpState()
		:
		direction(0.f),
		velocity(500.f),
		jump_y(0.f),
		fart("fart0.wav")
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;

private:
	float base_y;
	float jump_y;
	float velocity;
	float direction;
	Audio fart;
};

// ****************************************************************************
//You use this state when you're consuming or releasing water
class GettingWater : public IState<Player>
{
public:
  GettingWater()
    :
    slurp_sound("slurp.wav"),
    puke_sound("puke.wav")
  {}
	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;

  //The amount of water that it can consume/release simultaneously
  static float ConsumeWaterSpeed;
  static float ReleaseWaterSpeed;
  static float ChunkTime;
private:
	//The water that needs to be added to the waterpercentage
	float waterAdding;
  float last_update;
  Audio slurp_sound;
  Audio puke_sound;
};

class SmashState : public IState<Player>
{
public:
  SmashState()
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(::Input& input) override;

private:
};