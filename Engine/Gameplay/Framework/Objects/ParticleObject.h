#pragma once
#include "ArenaObject.h"
#include "MatrixDef.h"

class ParticleObjectBase
{
public:
  static class b2ParticleSystem* system;
};

class ParticleHandle : private ParticleObjectBase
{
public:
  ParticleHandle(size_t handle)
    :
    handle(handle)
  {}

  void SetPosition(Vec2& pos);
  Vec2 GetPosition();

private:
  size_t handle;
};

class ParticleObject : public ArenaObject<ParticleObject>
{
public:
  ParticleObject();

  void SetPosition(Vec2& pos);
  void Update();
  void Draw(class Renderer& gfx);
private:
  ParticleHandle handle;
};