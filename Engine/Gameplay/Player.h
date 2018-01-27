#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Utility\DrawUtils.h"
#include "Utility\SpriteAnimation.h"
#include "Keyboard.h"
#include "Physics\Body.h"

// Should be moved somewhere else
static const size_t NUM_PLAYERS = 4;
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
    Run,
    Jump,
  };

public:
  Player(Physics& simulation, TileGrid& grid, int id);

  virtual void Update(float dt);
  void Input(Input& input);
  void Draw()
  {
    b2Vec2 pos = b2Vec2(x, y);
    b2Vec2 size = b2Vec2(width, height);
    graphics[(int)curr_sprite].Draw(pos, size, flip_sprite, true);
  }

  void SetSprite(Sprite sprite)
  {
    curr_sprite = sprite;
  }
  void SetFlipped(bool flipped)
  {
    flip_sprite = flipped;
  }

  bool IsStuck();
  void LoseWater();

public:
  float x, y;
  float width, height;
  const int player_id;
  bool is_god;
  bool dead;
  Body physics_body;
  float waterPercentage = 0;
  float speed = 2.f;
private:
  TileGrid & grid;

  StateMachine<Player> movement;
  SpriteAnimation graphics[4];

  Font font;

  Sprite curr_sprite;
  bool flip_sprite;

};

// ****************************************************************************
class IdleState : public IState<Player>
{
public:
  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(::Input& input) override;

};
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
class MoveDownSideState : public IState<Player>
{
public:
  MoveDownSideState(Vec2 direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(::Input& input) override;

private:
  Vec2 direction;

};
// ****************************************************************************
class MoveUpDownState : public IState<Player>
{
public:
  MoveUpDownState(Vec2 direction)
    : direction(direction)
  {}

  State Update(float dt) override;
  State Input(::Input& input) override;

private:
  b2Vec2 direction;
};

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

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update(float dt) override;
	State Input(::Input& input) override;
private:
	//The amount of water that it can consume/release simultaneously
	float consumingWaterAmount;
	float releaseWaterAmount;
	//The water that needs to be added to the waterpercentage
	float waterAdding;
};