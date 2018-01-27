#pragma once
#include "Matrix.h"
#include "VArray.h"
#include "VMap.h"
#include "VString.h"
#include "Body.h"
#include "PhysicsConstants.h"
#include "LiquidFun\Box2D.h"

using World = b2World;
using PhysicsBody = b2Body;
using BodyDef = b2BodyDef;
using Fixture = b2Fixture;
using FixtureDef = b2FixtureDef;
using Shape = b2Shape;
using CircleShape = b2CircleShape;
using PolygonShape = b2PolygonShape;

enum class BodyType
{
  STATIC = b2BodyType::b2_staticBody,
  KINEMATIC = b2BodyType::b2_kinematicBody,
  DYNAMIC = b2BodyType::b2_dynamicBody,
};


class Physics
{
public:
  Physics();
  Body CreateBody(Vec2 pos = Vec2(0, 0), StringRef shape = "Circle", BodyType type = BodyType::DYNAMIC);
  size_t CreateParticle(Vec2 pos = Vec2(0, 0), ParticleFlag flag = ParticleFlag::b2_waterParticle);
  void AddShape(Shape* shape, StringRef name);
  
  
  
  void Update();
  
  void CreateDebugDraw(class Graphics& gfx);
  void DebugDraw();

private:
  b2ParticleSystem* particle_system;
  World world;
  Map<StringRef, Shape*> shapes;
  Array<BodyDef> body_defs;
  Array<FixtureDef> fixture_defs;
  float time_step = 1.0f / 60.0f;
};