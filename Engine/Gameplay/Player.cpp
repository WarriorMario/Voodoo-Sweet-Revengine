#include "Player.h"

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
IdleState::State IdleState::Input(Keyboard& input)
{
  float direction = 0.f;
  if(input.KeyIsPressed(VK_RIGHT))
  {
    direction += 1.f;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
    direction -= 1.f;
    Owner().SetFlipped(false);
  }
  if(direction != 0.f)
  {
    if(input.KeyIsPressed(VK_SHIFT))
    {
      return new RunState(direction);
    }
    else
    {
      return new MoveState(direction);
    }
  }
  if(input.KeyIsPressed(VK_SPACE))
  {
    return new JumpState;
  }

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
  Owner().x += direction * 5.f;

  return nullptr;
}
MoveState::State MoveState::Input(Keyboard& input)
{
  if(input.KeyIsPressed(VK_SPACE))
  {
    return new JumpState;
  }

  direction = 0.f;
  if(input.KeyIsPressed(VK_RIGHT))
  {
    direction += 1.f;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
    direction -= 1.f;
    Owner().SetFlipped(false);
  }
  if(direction == 0.f)
  {
    return new IdleState;
  }
  else if(input.KeyIsPressed(VK_SHIFT))
  {
    return new RunState(direction);
  }

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
  Owner().x += direction * 7.f;

  return nullptr;
}
RunState::State RunState::Input(Keyboard& input)
{
  if(input.KeyIsPressed(VK_SPACE))
  {
    return new JumpState;
  }

  direction = 0.f;
  if(input.KeyIsPressed(VK_RIGHT))
  {
    direction += 1.f;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
    direction -= 1.f;
    Owner().SetFlipped(false);
  }
  if(direction == 0.f)
  {
    return new IdleState;
  }
  else if(!input.KeyIsPressed(VK_SHIFT))
  {
    return new MoveState(direction);
  }

  return nullptr;
}

// ****************************************************************************
void JumpState::OnEnter(Player& player)
{
  Base::OnEnter(player);

  base_y = player.y;

  fart.Play();

  Owner().SetSprite(Player::Jump);
}
void JumpState::OnExit()
{
  Owner().y = base_y;
}

JumpState::State JumpState::Update()
{
  jump_y += velocity * 0.015f;

  velocity -= 25.f; // fake drag
  velocity = Max(velocity, -500.f); // fake terminal velocity

  if(jump_y < 0.f)
  {
    return new IdleState;
  }
  Owner().y = base_y - jump_y;

  Owner().x += direction * 5.f;

  return nullptr;
}
JumpState::State JumpState::Input(Keyboard& input)
{
  direction = 0.f;
  if(input.KeyIsPressed(VK_RIGHT))
  {
    direction += 1.f;
    Owner().SetFlipped(true);
  }
  if(input.KeyIsPressed(VK_LEFT))
  {
    direction -= 1.f;
    Owner().SetFlipped(false);
  }

  return nullptr;
}

// ****************************************************************************
Player::Player(Renderer& renderer)
  :
  renderer(renderer),
  movement(*this, new IdleState),
  sprites{"Images/idle.png", "Images/move.png", "Images/run.png", "Images/jump.png"},
  flip_sprite(false)
{
  width = 1920.f;
  height = 1080.f;
  x = 1920.f * 0.5f;
  y = 1080.f * 0.5f;
}
