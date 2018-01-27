#pragma once
#include "TileGrid.h"
#include "VArray.h"
#include "VString.h"
#include "Utility\Timer.h"
#include "Gameplay\Player.h"
#include "Physics\Physics.h"

class Renderer;
class Keyboard;
class Scene
{
public:
	Scene();

	void Init();
	void Tick(float dt, Keyboard& kbd);
	void Draw();

  // Gets the current god player
  Player* GetGod() const;
  // Gets the current players
  Array<Player*> GetPlayers() const;
  // Demotes the current god and assigns a new player as god
  void NewGod();

  void KillPlayer(int idx);
  void SpawnPlayer(int idx, Vec2 pos);

private:
	// to manage the playing field
	static constexpr char LEVEL_TO_LOAD[] = "Levels/Debug.json";
	TileGrid tile_grid;

	// players
	Player* players[4];

	// physics
	Physics physx;

};