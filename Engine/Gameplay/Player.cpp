#include "Player.h"
#include "Input\Input.h"
#include "Physics\Physics.h"
#include "TileGrid.h"
#include "Tile.h"
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
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x > 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x;
    Owner().SetFlipped(false);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y;
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
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).x < 0)
  {
    dir.x += input.GetAxis(AxisCode::LEFT, Owner().player_id).x;
    Owner().SetFlipped(false);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y > 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y;
    Owner().SetFlipped(true);
  }
  if(input.GetAxis(AxisCode::LEFT, Owner().player_id).y < 0)
  {
    dir.y += input.GetAxis(AxisCode::LEFT, Owner().player_id).y;
    Owner().SetFlipped(false);
  }
  if(dir.x == 0.f && dir.y)
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
#define MOVE_LAYER_0 "Images/God/Layer0/0_Kid1"
#define MOVE_LAYER_1 "Images/God/Layer1/1_Kid1"
#define MOVE_LAYER_2 "Images/God/Layer2/2_Kid1"
#define MOVE_LAYER_3 "Images/God/Layer3/3_Kid1"

Player::Player(Physics& simulation, TileGrid& grid, int id)
  :
  movement(*this, new IdleState),
  flip_sprite(false),
  physics_body(simulation.CreateBody(Vec2(40, 30), "Square")),
  grid(grid),
  player_id(id),
  graphics{ 0.1f, 0.1f, 0.1f, 0.1f }
{
  graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_0, 3);
  graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_1, 3);
  graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_2, 3);
  graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_3, 3);

  width = 32.0f;
  height = 32.0f;
  x = 400.0f;
  y = 300.0f;
  physics_body.body->SetUserData(this);
  dead = false;
}

void Player::Update(float dt)
{
  if(dead)
  {
    return;
  }

  total_time += dt;

  graphics[(int)Sprite::Move].ScaleLayer(0, 2.0f);
  graphics[(int)Sprite::Move].ScaleLayer(1, 2.0f);
  graphics[(int)Sprite::Move].ScaleLayer(2, 2.0f);
  graphics[(int)Sprite::Move].ScaleLayer(3, 2.0f);

  float t = sinf(total_time * 2.0f) / 2.0f + 0.5f;
  float scale = t + 2.f;
  graphics[(int)Sprite::Move].ScaleLayer(1, scale);
  graphics[(int)Sprite::Move].OffsetLayer(1, Vec2(0, -100 * t));

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
  return grid.IsPassable(x / Tile::SIZE, y / Tile::SIZE) == false;
}

void Player::LoseWater()
{
  // Not implemented yet
  dead = true;
}
