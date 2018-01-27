#pragma once
#include "Physics\Body.h"

class Physics;

class b2Vec2;
using Vec2 = b2Vec2;
class b2Shape;
using Shape = b2Shape;
struct b2FixtureDef;
using FixtureDef = b2FixtureDef;
class b2Fixture;
using Fixture = b2Fixture;

class Blockade
{
public:
	Blockade(Physics* physx);

	void SetPosition(const Vec2& pos);
	void SetShape(Shape* new_shape);
	void SetFixtureDefinition(const FixtureDef& definition);
	void Update();

private:
	Body body;
	Shape* shape;
	Fixture* fixture;
	Physics* physx;
};
