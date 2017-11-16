#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Graphsicks\Renderer.h"
#include "Keyboard.h"

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

    const float min_u = flip_sprite ? 1.f : 0.f;
    const float max_u = flip_sprite ? 0.f : 1.f;
    
    ForegroundShader::PrimData vertices[3];
    // left-top tri
    // pcs
    vertices[0].x = x - 0.5f * width;
    vertices[0].y = y - 0.5f * height;
    vertices[1].x = x + 0.5f * width;
    vertices[1].y = y - 0.5f * height;
    vertices[2].x = x - 0.5f * width;
    vertices[2].y = y + 0.5f * height;
    // uvs
    vertices[0].u = min_u;
    vertices[0].v = 0.f;
    vertices[1].u = max_u;
    vertices[1].v = 0.f;
    vertices[2].u = min_u;
    vertices[2].v = 1.f;

    ForegroundShader draw_call = ForegroundShader(
    vertices[0], vertices[1], vertices[2],
      Colors::White, &sprites[curr_sprite]);

    renderer.AddDrawCommand(draw_call);
    
    // bot-right tri
    // pcs
    vertices[0].x = x + 0.5f * width;
    vertices[0].y = y - 0.5f * height;
    vertices[1].x = x + 0.5f * width;
    vertices[1].y = y + 0.5f * height;
    vertices[2].x = x - 0.5f * width;
    vertices[2].y = y + 0.5f * height;
    // uvs
    vertices[0].u = max_u;
    vertices[0].v = 0.f;
    vertices[1].u = max_u;
    vertices[1].v = 1.f;
    vertices[2].u = min_u;
    vertices[2].v = 1.f;

    draw_call = ForegroundShader(
      vertices[0], vertices[1], vertices[2],
      Colors::White, &sprites[curr_sprite]);

    renderer.AddDrawCommand(draw_call);
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
  Renderer& renderer;
  
  StateMachine<Player> movement;
  Texture sprites[4];

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
  MoveState(float direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

private:
  float direction;

};
class RunState : public IState<Player>
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

private:
  float direction;

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
