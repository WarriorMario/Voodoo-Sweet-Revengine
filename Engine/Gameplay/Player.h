#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Utility\DrawUtils.h"
#include "Keyboard.h"

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
  Player(Renderer& renderer);

  void Update()
  {
    movement.Update();
  }
  void Input(Keyboard& input)
  {
    movement.Input(input);
  }
  void Draw()
  {
	b2Vec2 pos = b2Vec2(x - 0.5f * width, y - 0.5f * height);
	b2Vec2 size = b2Vec2(width, height);
	RenderQuad<ForegroundShader>(renderer, pos, size,
		flip_sprite, false, Colors::Cyan, &sprites[curr_sprite]);
	RenderText(renderer, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		font, 24, b2Vec2(200, 200), Colors::Red);
	RenderText(renderer, "., -=+\\ / ()*&^@#!_>< ? %~: ; \"'_",
		font, 24, b2Vec2(200, 100), Colors::Red);
  }

  void SetSprite(Sprite sprite)
  {
    curr_sprite = sprite;
  }
  void SetFlipped(bool flipped)
  {
    flip_sprite = flipped;
  }
  /*bool CollisionCheck(Player player) {
	  player.topLeft = Vec2(player.x, player.y);
	  player.downRight = Vec2(player.x+player.width, player.y+player.height);
	  player.topLeft = Vec2(player.x + player.width, player.y);
	  player.topRight = Vec2(player.x + player.width, player.y);
	  topLeft = Vec2(x, y);
	  downRight = Vec2(x + width, y + height);
	  topLeft = Vec2(x + width, y);
	  topRight = Vec2(x + width, y);
	  if (b2DistanceSquared(player.topRight, topLeft) < 10 || b2DistanceSquared(player.downRight, downLeft) < 10 || b2DistanceSquared(player.topLeft, topRight) || b2DistanceSquared(player.downLeft, downRight)) {
		  return true;
	  }
	  return false;
  }*/
public:
  float x, y;
  float width, height;
  float speed;
  float waterPercentage;
  bool ischild;
  Vec2 topLeft;
  Vec2 downRight;
  Vec2 topRight;
  Vec2 downLeft;
private:
  Renderer& renderer;
  
  StateMachine<Player> movement;
  Texture sprites[5];

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

  State Update() override;
  State Input(Keyboard& input) override;

};

// ****************************************************************************
class MoveSideState : public IState<Player>
{
public:
  MoveSideState(float direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

private:
  float direction;

};

// ****************************************************************************
class MoveUpSideState : public IState<Player>
{
public:
	MoveUpSideState(float direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	float direction;

};

// ****************************************************************************
class MoveDownSideState : public IState<Player>
{
public:
	MoveDownSideState(float direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	float direction;

};
// ****************************************************************************
class MoveUpDownState : public IState<Player>
{
public:
	MoveUpDownState(float direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	float direction;

};

// ****************************************************************************
/*class RunState : public IState<Player>
{
public:
  RunState(float direction)
    :
    direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;
  bool isGettingWater;
  bool isDroppingWater;
  float waterPercentage;
  float speed;
private:
  float direction;
};*/

// ****************************************************************************
class GettingWater : public IState<Player>
{
public:
	GettingWater(float direction)
		:
		direction(direction)
	{}
	void OnEnter(Player& player) override;
	void OnExit() override;
	
	State Update() override;
	State Input(Keyboard& input) override;
private:
	float gettingWaterAmount;
	float releaseWaterAmount;
	float waterAdding;
	float speed;
	float direction;
};