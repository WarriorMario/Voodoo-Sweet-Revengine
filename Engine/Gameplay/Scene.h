#pragma once
#include "TileGrid.h"
#include "VString.h"

class Renderer;
class Scene
{
public:
	Scene();
	void Init();
	void Tick(float delta_time);
	void Draw(Renderer& renderer);

private:
	static constexpr char LEVEL_TO_LOAD[] = "Levels/Debug.json";
	TileGrid tile_grid;
	// data stuffs
};