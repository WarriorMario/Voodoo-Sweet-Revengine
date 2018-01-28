#pragma once
#include "Matrix.h"
#include "VArray.h"
#include "Graphsicks\Graphics.h"

class Player;

class Camera
{
public:
  void CalculateOffset(const Array<Player*>& players);
  Vec2 Transform(Vec3 vector);

  Vec2 offset = Vec2(0.0f, 0.0f);
  float zoom = 1.0f;

};