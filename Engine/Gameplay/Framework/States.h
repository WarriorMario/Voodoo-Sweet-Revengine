#pragma once

#include "Utility.h"

// ****************************************************************************
class Input;

// ****************************************************************************
template<typename T>
class IState
{
protected:
  using Base = IState<T>;
  using State = IState<T>*;

public:
  virtual void OnEnter(T& in_owner)
  {
    owner = &in_owner;
  }
  virtual void OnExit() = 0;

  virtual State Update(float dt) = 0;
  virtual State Input(Input& input) = 0;

protected:
  T & Owner()
  {
    return *owner;
  }

private:
  T * owner;

};

// ****************************************************************************
template<typename T>
class StateMachine
{
  using State = IState<T>*;

public:
  StateMachine(T& owner, State initial_state)
    :
    owner(owner),
    state(initial_state)
  {
    assert(state && "Invalid intial state");
    state->OnEnter(owner);
  }
  StateMachine(const StateMachine&) = delete;
  ~StateMachine()
  {
    state->OnExit();
    delete state;
  }

  void Update(float dt)
  {
    State new_state = state->Update(dt);
    if(new_state)
    {
      state->OnExit();
      delete state;

      state = new_state;
      state->OnEnter(owner);
    }
  }
  void Input(Input& input)
  {
    State new_state = state->Input(input);
    if(new_state)
    {
      state->OnExit();
      delete state;

      state = new_state;
      state->OnEnter(owner);
    }
  }

private:
  T & owner;
  State state;

};
