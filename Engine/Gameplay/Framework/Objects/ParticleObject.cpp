#include "ParticleObject.h"
#include "..\Arena.h"
#include "Graphsicks\Renderer.h"
#include "Physics\PhysicsConstants.h"
#include "Utility\DrawUtils.h"

 b2ParticleSystem* ParticleObjectBase::system = nullptr;
 Texture* water;

ParticleObject::ParticleObject()
  :
  TemplateBase("ParticleObject"),
  handle(arena->physx.CreateParticle())
{
  static bool test = false;
  if(test == false)
  {
    test = true;
    water = new Texture("Images/water.png");
  }
}

void ParticleObject::SetPosition(Vec2& pos)
{
  handle.SetPosition(pos / PHYSICS_SCALE);
}

void ParticleObject::Update()
{}

void ParticleObject::Draw(Renderer & gfx)
{
  Vec2 pos = handle.GetPosition();
  Vec2 size = {64,64};
  BackgroundShader::PrimData vertices[3];
  vertices[0] = {100 + pos.x, 100 + pos.y, 0.0, 0.0f};
  vertices[1] = {100 + pos.x, 00 + pos.y, 1.0f, 0.0f};
  vertices[2] = {200 + pos.x, 00 + pos.y, 0.5f, 1.0f};
  RenderQuad<ForegroundShader>(gfx, pos, size, false, false, Colors::White, water);
}

void ParticleHandle::SetPosition(Vec2 & pos)
{
  system->GetPositionBuffer()[handle] = pos;
}

Vec2 ParticleHandle::GetPosition()
{
  return system->GetPositionBuffer()[handle] * PHYSICS_SCALE;
}
