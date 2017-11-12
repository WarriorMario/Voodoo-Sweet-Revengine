#include "ParticleObject.h"
#include "..\Arena.h"
#include "Vec2.h"
ParticleObject::ParticleObject()
  :
  TemplateBase("ParticleObject"),
  handle(arena->physx.CreateParticle())
{}

void ParticleObject::SetPosition(Vec2& pos)
{
  arena->physx.particle_system->GetPositionBuffer()[handle] = pos;
}

void ParticleObject::Update()
{}
