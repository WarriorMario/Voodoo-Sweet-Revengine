#pragma once
#include "LiquidFun\Box2D.h"
#include "Vec2.h"
#include "VArray.h"
#include "VMap.h"
#include "VString.h"
#include "Body.h"

using World = b2World;
using PhysicsBody = b2Body;
using BodyDef = b2BodyDef;
using Fixture = b2Fixture;
using FixtureDef = b2FixtureDef;
using Shape = b2Shape;
using CircleShape = b2CircleShape;

enum class BodyType
{
  STATIC = b2BodyType::b2_staticBody,
  KINEMATIC = b2BodyType::b2_kinematicBody,
  DYNAMIC = b2BodyType::b2_dynamicBody,
};


class Physics
{
public:
  Physics()
    :
    world(Vec2(0,9.81))
  {
    static CircleShape shape;
    shape.m_radius = 3;

    AddShape(&shape,"Circle");
  }
  Body CreateBody(Vec2 pos = Vec2(0, 0), StringRef shape = "Circle", BodyType type = BodyType::DYNAMIC);
  void AddShape(Shape* shape, StringRef name);
  void Update();
private:
  World world;
  Map<StringRef,Shape*> shapes;
  Array<BodyDef> body_defs;
  Array<FixtureDef> fixture_defs;
  float time_step = 1.0f / 60.0f;
};