#pragma once
#include "VString.h"
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
	unsigned int width;
	unsigned int height;
	Tile* tiles;
};