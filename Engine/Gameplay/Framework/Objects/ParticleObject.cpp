#include "ParticleObject.h"
#include "..\Arena.h"
#include "Vec2.h"
#include "Graphsicks\Renderer.h"
#include "Physics\PhysicsConstants.h"
 b2ParticleSystem* ParticleObjectBase::system = nullptr;


ParticleObject::ParticleObject()
  :
  TemplateBase("ParticleObject"),
  handle(arena->physx.CreateParticle())
{}

void ParticleObject::SetPosition(Vec2& pos)
{
  handle.SetPosition(pos / PHYSICS_SCALE);
}

void ParticleObject::Update()
{}

void ParticleObject::Draw(Renderer & gfx)
{
  Vec2 pos = handle.GetPosition();
  BackgroundShader testASDASD;
  testASDASD.const_data.color = Colors::Yellow;
  testASDASD.prim_data[0] = {100 + pos.x, 100 + pos.y, 0.0, 0.0f};
  testASDASD.prim_data[1] = {100 + pos.x, 00 + pos.y, 1.0f, 0.0f};
  testASDASD.prim_data[2] = {200 + pos.x, 00 + pos.y, 0.5f, 1.0f};
  gfx.AddDrawCommand(testASDASD);


}

void ParticleHandle::SetPosition(Vec2 & pos)
{
  system->GetPositionBuffer()[handle] = pos;
}

Vec2 ParticleHandle::GetPosition()
{
  return system->GetPositionBuffer()[handle] * PHYSICS_SCALE;
}
