#include "Physics.h"
#include "DebugDraw.h"
#include "Gameplay\Framework\Objects\ParticleObject.h"

Physics::Physics()
  :
  world(b2Vec2(0, 0))
{
  b2ParticleSystemDef particle_system_def;
  particle_system_def.strictContactCheck = false;
  particle_system_def.density = 1.0f;
  particle_system_def.gravityScale = 1.0f;
  particle_system_def.radius = 1.0f;
  particle_system_def.maxCount = 0;

  particle_system_def.pressureStrength = 0.05f;
  particle_system_def.dampingStrength = 1.0f;
  particle_system_def.elasticStrength = 0.25f;
  particle_system_def.springStrength = 0.25f;
  particle_system_def.viscousStrength = 0.25f;
  particle_system_def.surfaceTensionPressureStrength = 0.2f;
  particle_system_def.surfaceTensionNormalStrength = 0.2f;
  particle_system_def.repulsiveStrength = 1.0f;
  particle_system_def.powderStrength = 0.5f;
  particle_system_def.ejectionStrength = 0.5f;
  particle_system_def.staticPressureStrength = 0.2f;
  particle_system_def.staticPressureRelaxation = 0.2f;
  particle_system_def.staticPressureIterations = 8;
  particle_system_def.colorMixingStrength = 0.5f;
  particle_system_def.destroyByAge = true;
  particle_system_def.lifetimeGranularity = 1.0f / 60.0f;
  particle_system = world.CreateParticleSystem(&particle_system_def);

  ParticleObjectBase::system = particle_system;
  static CircleShape shape;
  shape.m_radius = 3;

  AddShape(&shape, "Circle");
}

Body Physics::CreateBody(Vec2 pos, StringRef shape , BodyType type )
{
  BodyDef def;
  def.type = (b2BodyType)type;
  def.position = pos;
  body_defs.push_back(def);// This might not be required?
  
  FixtureDef fixture_def;
  fixture_def.shape = shapes[shape];
  fixture_defs.push_back(fixture_def);
  PhysicsBody*ret  = world.CreateBody(&def);
  ret->CreateFixture(&fixture_def);
  return Body(ret);
}

size_t Physics::CreateParticle(Vec2 pos, ParticleFlag flag)
{
  b2ParticleDef particle_def;
  particle_def.position = pos;
  particle_def.flags = flag;

  return particle_system->CreateParticle(particle_def);
}

void Physics::AddShape(Shape * shape, StringRef name)
{
	shapes[name] = shape;
}

void Physics::Update()
{
  world.Step(time_step, 1, 1);
}

void Physics::CreateDebugDraw(Graphics & gfx)
{
  world.SetDebugDraw(new PhysicsDebugDraw(gfx));
}

void Physics::DebugDraw()
{
  world.DrawDebugData();

}
