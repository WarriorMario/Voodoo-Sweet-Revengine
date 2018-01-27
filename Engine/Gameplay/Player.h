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
		Move,
		Run,
		Jump,
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

private:

	StateMachine<Player> movement;
	Texture sprites[4];

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
class MoveState : public IState<Player>
{
public:
	MoveState(b2Vec2 direction)
		:
		dir(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 dir;
};

class RunState : public IState<Player>
{
public:
	RunState(b2Vec2 direction)
		:
		dir(direction)
	{}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	b2Vec2 dir;
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
	{
	}

	void OnEnter(Player& player) override;
	void OnExit() override;

	State Update() override;
	State Input(Keyboard& input) override;

private:
	float base_y;
	float jump_y;
	float velocity;
	float direction;
	Audio fart;
};
