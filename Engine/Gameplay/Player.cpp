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

IdleState::State IdleState::Update()
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

MoveState::State MoveState::Update()
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

RunState::State RunState::Update()
{
  dir.Normalize();
  Owner().x += dir.x * 10.f;
  Owner().y += dir.y * 10.f;

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

JumpState::State JumpState::Update()
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
Player::Player(Physics& simulation, TileGrid& grid, int id)
  :
  movement(*this, new IdleState),
  sprites{"Images/idle.png", "Images/move.png", "Images/run.png", "Images/jump.png"},
  flip_sprite(false),
  font(Font("Fonts/times.ttf")),
  physics_body(simulation.CreateBody(Vec2(40, 30), "Square")),
  grid(grid),
  player_id(id)
{
  width = 32.0f;
  height = 32.0f;
  x = 400.0f;
  y = 300.0f;
  physics_body.body->SetUserData(this);
  dead = false;
}

void Player::Update()
{
  if(dead)
  {
    return;
  }
  movement.Update();
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
