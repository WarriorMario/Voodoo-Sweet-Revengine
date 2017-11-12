#pragma once
#include "ArenaObject.h"

class ParticleObject : public ArenaObject<ParticleObject>
{
public:
  ParticleObject();

  void SetPosition(struct Vec2& pos);
  void Update();
private:
  size_t handle;
};