#pragma once
#include "ArenaObject.h"

class ParticleObjectBase
{
public:
  static class b2ParticleSystem* system;
};

class ParticleObject : public ArenaObject<ParticleObject>, private ParticleObjectBase
{
public:
  ParticleObject();

  void SetPosition(struct Vec2& pos);
  void Update();
  void Draw(class Renderer& gfx);
private:
  size_t handle;
};