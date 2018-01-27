#include "Blockade.h"
#include "Physics\Physics.h"
#include "Graphsicks\Renderer.h"
#include "Physics\PhysicsConstants.h"

Blockade::Blockade(Physics* physx)
	:
	body(physx->CreateBody())
{
}

void Blockade::SetPosition(const Vec2& pos)
{
	body.body->SetTransform(pos / PHYSICS_SCALE, body.body->GetAngle());
}

void Blockade::SetShape(Shape * new_shape)
{
	FixtureDef new_definition;
	new_definition.density = fixture->GetDensity();
	new_definition.filter = fixture->GetFilterData();
	new_definition.friction = fixture->GetFriction();
	new_definition.isSensor = fixture->IsSensor();
	new_definition.restitution = fixture->GetRestitution();
	new_definition.userData = fixture->GetUserData();
	new_definition.shape = new_shape;
	body.body->DestroyFixture(fixture);
	fixture = body.body->CreateFixture(&new_definition);
}

void Blockade::SetFixtureDefinition(const FixtureDef & definition)
{
	body.body->DestroyFixture(fixture);
	fixture = body.body->CreateFixture(&definition);
}

void Blockade::Update()
{

}
