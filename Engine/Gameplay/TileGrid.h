#pragma once
#include "VString.h"

class Renderer;
class Tile;

class TileGrid
{
public:
	// default and instant initialization constructor for convenience
	TileGrid();
	TileGrid(StringRef level_name);

	// rule of five shizzles
	TileGrid(const TileGrid& rhs);
	TileGrid(TileGrid&& rhs);
	TileGrid& operator=(const TileGrid& rhs);
	TileGrid& operator=(TileGrid&& rhs);

	// destructor
	~TileGrid();

	// destroys any present data, creates a new level
	bool LoadLevel(StringRef level_name);

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

private:
	static constexpr char TEXTURE_TO_LOAD[] = "Images/TileAtlas/DebugAtlas.bmp";
	unsigned int width;
	unsigned int height;
	Tile* tiles;
};