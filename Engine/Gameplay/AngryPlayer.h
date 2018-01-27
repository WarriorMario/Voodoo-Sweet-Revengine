#pragma once
#include "Player.h"

class Physics;
class TileGrid;
class AngryPlayer : public Player
{
  public:
    AngryPlayer(Physics& simulation, TileGrid& grid, int id);
    void Update(float dt) override;
};