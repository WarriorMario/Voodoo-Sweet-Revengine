#pragma once
#include "ArenaObject.h"
#include "Physics\Body.h"
class b2Shape;
using Shape = b2Shape;
struct b2FixtureDef;
using FixtureDef = b2FixtureDef;
class b2Fixture;
using Fixture = b2Fixture;

struct b2Vec2;
using Vec2 = b2Vec2;

class PhysicsObject : public ArenaObject<PhysicsObject>
{
public:
  PhysicsObject();

  void SetPosition(const Vec2& pos);
  void SetShape(Shape* new_shape);
  void SetFixtureDefinition(const FixtureDef& definition);
  void Update();

private:
  Body body;
  Shape* shape;
  Fixture* fixture;
};
