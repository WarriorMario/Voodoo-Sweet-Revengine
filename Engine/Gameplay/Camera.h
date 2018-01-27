#pragma once
#include "Matrix.h"
#include "VArray.h"

class Player;

class Camera
{
public:
  void CalculateOffset(const Array<Player*>& players);
  Vec2 offset;
};