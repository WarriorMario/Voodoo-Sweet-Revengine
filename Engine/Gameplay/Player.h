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
    ForegroundShader draw_call;
    draw_call.const_data.color = Colors::White;
    draw_call.const_data.texture = &sprites[curr_sprite];

    const float min_u = flip_sprite ? 1.f : 0.f;
    const float max_u = flip_sprite ? 0.f : 1.f;
    
    // left-top tri
    // pcs
    draw_call.prim_data[0].x = x - 0.5f * width;
    draw_call.prim_data[0].y = y - 0.5f * height;
    draw_call.prim_data[1].x = x + 0.5f * width;
    draw_call.prim_data[1].y = y - 0.5f * height;
    draw_call.prim_data[2].x = x - 0.5f * width;
    draw_call.prim_data[2].y = y + 0.5f * height;
    // uvs
    draw_call.prim_data[0].u = min_u;
    draw_call.prim_data[0].v = 0.f;
    draw_call.prim_data[1].u = max_u;
    draw_call.prim_data[1].v = 0.f;
    draw_call.prim_data[2].u = min_u;
    draw_call.prim_data[2].v = 1.f;

    renderer.AddDrawCommand(draw_call);
    
    // bot-right tri
    // pcs
    draw_call.prim_data[0].x = x + 0.5f * width;
    draw_call.prim_data[0].y = y - 0.5f * height;
    draw_call.prim_data[1].x = x + 0.5f * width;
    draw_call.prim_data[1].y = y + 0.5f * height;
    draw_call.prim_data[2].x = x - 0.5f * width;
    draw_call.prim_data[2].y = y + 0.5f * height;
    // uvs
    draw_call.prim_data[0].u = max_u;
    draw_call.prim_data[0].v = 0.f;
    draw_call.prim_data[1].u = max_u;
    draw_call.prim_data[1].v = 1.f;
    draw_call.prim_data[2].u = min_u;
    draw_call.prim_data[2].v = 1.f;

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
