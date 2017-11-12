#include "Physics.h"

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

void Physics::AddShape(Shape * shape, StringRef name)
{
	shapes[name] = shape;
}

void Physics::Update()
{
  world.Step(time_step, 1, 1);
}
