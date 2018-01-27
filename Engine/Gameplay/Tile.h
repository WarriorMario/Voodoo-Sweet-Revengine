#pragma once
#include "Matrix.h"

class Texture;
class Renderer;
class Physics;
class Tile
{
public:
	enum TileFunction
	{
		DEFAULT,
		COLLISION,
		BORDER,
		GOD_SPAWN_LOCATION,
		PLAYER_SPAWN_LOCATION,
		WATER
	};
public:

	Tile();

	void Init(int grid_pos_x, int grid_pos_y, int tile_visual,
		TileFunction tile_function, Texture* atlas_texture, Physics& simulation);
	void InitFunction(TileFunction function, Physics& simulation);
	void Render();
	b2Vec2 GetPos()
	{
		return b2Vec2(pos_x, pos_y);
	}
	bool IsPassable()
	{
		return function != COLLISION && function != WATER;
	}
	
	static constexpr unsigned int SIZE = 64;

private:
	static constexpr int TILE_SIZE_IN_ATLAS = 33;
	float min_u, max_u, min_v, max_v;
	int pos_x, pos_y;
	int visual;
	TileFunction function;
	Texture* atlas;
};