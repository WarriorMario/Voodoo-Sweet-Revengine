#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Utility\DrawUtils.h"
#include "Utility\SpriteAnimation.h"
#include "Keyboard.h"

// Should be moved somewhere else
static const size_t NUM_PLAYERS = 4;

// ****************************************************************************
class Player
{
public:

  enum Sprite
  {
    Idle,
    MoveSide,
    MoveUp,
    MoveUpSide,
    MoveDownSide
  };


public:
  Player();

  void Update(float dt)
  {
    movement.Update(dt);
    graphics[(int)curr_sprite].Update(dt);
  }
  void Input(Keyboard& input)
  {
    movement.Input(input);
  }
  void Draw()
  {
    Vec2 pos = Vec2(x, y);
    Vec2 size = Vec2(width, height);
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

public:
  float x, y;
  float width, height;
  float speed;
  float waterPercentage;
  bool is_god;

private:
  StateMachine<Player> movement;
  SpriteAnimation graphics[5];

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
  State Input(Keyboard& input) override;

};

// ****************************************************************************
class MoveSideState : public IState<Player>
{
public:
  MoveSideState(Vec2 direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(Keyboard& input) override;

private:
  Vec2 direction;
};

// ****************************************************************************
class MoveUpSideState : public IState<Player>
{
public:
  MoveUpSideState(Vec2 direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(Keyboard& input) override;

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
  State Input(Keyboard& input) override;

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

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(Keyboard& input) override;

private:
  Vec2 direction;
};

// ****************************************************************************
class GettingWater : public IState<Player>
{
public:
  GettingWater(Vec2 direction)
    :
    direction(direction)
  {}
  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update(float dt) override;
  State Input(Keyboard& input) override;

private:
  float gettingWaterAmount;
  float releaseWaterAmount;
  float waterAdding;
  float speed;
  Vec2 direction;

};
