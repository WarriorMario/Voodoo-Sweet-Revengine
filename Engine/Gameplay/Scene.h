#pragma once
#include "TileGrid.h"
#include "VString.h"
#include "Utility\Timer.h"
#include "Gameplay\Player.h"
#include "Physics\Physics.h"

class Renderer;
class Graphics;
class Input;
class Scene
{
	// doesn't alter any objects, only needs access to the player
	friend class FrameCounter;
public:
	Scene();

	void Init(Graphics& gfx);
	void Tick(Input& kbd);
	void Draw(Renderer& renderer);
  void DebugDraw();

private:
	// to keep track of the game's speed
	Timer frame_timer;

	// to manage the playing field
	static constexpr char LEVEL_TO_LOAD[] = "Levels/Debug.json";
	TileGrid tile_grid;

	// players
	Player* players[NUM_PLAYERS];

	// physics
	Physics simulation;
};