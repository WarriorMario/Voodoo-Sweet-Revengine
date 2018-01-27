#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Utility\DrawUtils.h"
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

	void Update()
	{
		movement.Update();
	}
	void Input(Keyboard& input)
	{
		movement.Input(input);
	}
	void Draw(Renderer& renderer)
	{
		b2Vec2 pos = b2Vec2(x, y);
		b2Vec2 size = b2Vec2(width, height);
		RenderQuad<ForegroundShader>(renderer, pos, size,
			flip_sprite, true, Colors::Cyan, &sprites[curr_sprite]);
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

public:
	float x, y;
	float width, height;
	float speed;
	float waterPercentage;
private:

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
  MoveSideState(b2Vec2 direction)
    : direction(direction)
  {}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 direction;
};

// ****************************************************************************
class MoveUpSideState : public IState<Player>
{
public:
	MoveUpSideState(b2Vec2 direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 direction;

};

// ****************************************************************************
class MoveDownSideState : public IState<Player>
{
public:
	MoveDownSideState(b2Vec2 direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 direction;

};
// ****************************************************************************
class MoveUpDownState : public IState<Player>
{
public:
	MoveUpDownState(b2Vec2 direction)
		: direction(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 direction;
};

// ****************************************************************************
class GettingWater : public IState<Player>
{
public:
	GettingWater(b2Vec2 direction)
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
	b2Vec2 direction;
};
