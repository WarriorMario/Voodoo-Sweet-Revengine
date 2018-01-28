#pragma once
#include "VString.h"
#include "VArray.h"
#include "Matrix.h"

class Renderer;
class Physics;
class Tile;

class TileGrid
{
public:
	// default and instant initialization constructor for convenience
	TileGrid();
	TileGrid(StringRef level_name, Physics& simulation);

	// rule of five shizzles
	TileGrid(const TileGrid& rhs);
	TileGrid(TileGrid&& rhs);
	TileGrid& operator=(const TileGrid& rhs);
	TileGrid& operator=(TileGrid&& rhs);

	// destructor
	~TileGrid();

	// destroys any present data, creates a new level
	bool LoadLevel(StringRef level_name, Physics& simulation);

	// renders all of it's tiles
	void Draw();

	// getters to access width and height
	inline unsigned int GetGridWidth()
	{
		return width;
	}
	inline unsigned int GetGridHeight()
	{
		return height;
	}
	inline unsigned int GetNumTiles()
	{
		return width * height;
	}
	bool IsPassable(int x, int y, bool is_neighbour);

	b2Vec2 GetRandomSpawnPlayer();
	b2Vec2 GetRandomSpawnGod();

private:
	static constexpr char TEXTURE_TO_LOAD[] = "Images/TileAtlas/spritesheet.jpg";
	unsigned int width;
	unsigned int height;
	Tile* tiles;

	Array<b2Vec2> player_spawn_locations;
	Array<b2Vec2> god_spawn_locations;
};