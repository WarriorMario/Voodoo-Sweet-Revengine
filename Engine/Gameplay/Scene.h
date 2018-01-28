#pragma once
#include "TileGrid.h"
#include "VArray.h"
#include "VString.h"
#include "Utility\Timer.h"
#include "Gameplay\Player.h"
#include "Physics\Physics.h"

class AngryPlayer;
class Renderer;
class Graphics;
class Input;
class Scene
{
public:
	Scene();

	void Init(Graphics& gfx);
	void Tick(float dt, Input& kbd);
	void Draw();

  // Gets the current god player
  AngryPlayer* GetGod() const;
  // Gets the current players
  Array<Player*> GetPlayers() const;
  // Demotes the current god and assigns a new player as god
  void NewGod();

  void KillPlayer(int idx);
  void SpawnPlayer(int idx);
  void DebugDraw();

private:
	// to manage the playing field
	static constexpr char LEVEL_TO_LOAD[] = "Levels/Debug.json";
	TileGrid tile_grid;

	// players
	Player* players[NUM_PLAYERS];

	// physics
	Physics simulation;
};