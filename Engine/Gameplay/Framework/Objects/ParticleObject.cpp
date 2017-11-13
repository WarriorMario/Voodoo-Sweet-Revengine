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
  system->GetPositionBuffer()[handle] = pos;
}

void ParticleObject::Update()
{}

void ParticleObject::Draw(Renderer & gfx)
{
  Vec2 pos = system->GetPositionBuffer()[handle]*PHYSICS_SCALE;
  BackgroundShader testASDASD;
  testASDASD.const_data.color = Colors::Yellow;
  testASDASD.prim_data[0] = {10 + pos.x, 10 + pos.y, 0.0, 0.0f};
  testASDASD.prim_data[1] = {10 + pos.x, 00 + pos.y, 1.0f, 0.0f};
  testASDASD.prim_data[2] = {20 + pos.x, 00 + pos.y, 0.5f, 1.0f};
  gfx.AddDrawCommand(testASDASD);


}
