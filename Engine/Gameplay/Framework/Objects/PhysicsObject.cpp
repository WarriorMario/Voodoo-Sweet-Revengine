#include "PhysicsObject.h"
#include "..\Arena.h"
#include "Physics\Physics.h"
#include "Graphsicks\Renderer.h"

PhysicsObject::PhysicsObject()
  :
  TemplateBase("Physics Object"),
  body(arena->physx.CreateBody())
{
}

void PhysicsObject::SetPosition(const Vec2& pos)
{
  body.body->SetTransform(pos, body.body->GetAngle());
}

void PhysicsObject::SetShape(Shape * new_shape)
{
  FixtureDef new_definition;
  new_definition.density = fixture->GetDensity();
  new_definition.filter = fixture->GetFilterData();
  new_definition.friction = fixture->GetFriction();
  new_definition.isSensor= fixture->IsSensor();
  new_definition.restitution= fixture->GetRestitution();
  new_definition.userData= fixture->GetUserData();
  new_definition.shape = new_shape;
  body.body->DestroyFixture(fixture);
  fixture = body.body->CreateFixture(&new_definition);
}

void PhysicsObject::SetFixtureDefinition(const FixtureDef & definition)
{
  body.body->DestroyFixture(fixture);
  fixture = body.body->CreateFixture(&definition);
}

void PhysicsObject::Update()
{

}

void PhysicsObject::Draw(Renderer& renderer)
{}
