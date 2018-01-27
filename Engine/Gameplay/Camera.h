#pragma once
#include "Matrix.h"
class Player;

class Camera
{
public:
  void CalculateOffset(Player** players, size_t num_players);
  Vec2 offset;
};