#pragma once
class b2Body;
using PhysicsBody = b2Body;

class Body
{
public:
  Body(PhysicsBody * body)
    :
    body(body)
  {}
  PhysicsBody* body;
};
