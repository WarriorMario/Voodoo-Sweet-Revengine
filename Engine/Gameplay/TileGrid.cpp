#include "TileGrid.h"
#include "Tile.h"
#include "Utility\Serialization.h"
#include <assert.h>

TileGrid::TileGrid()
	:
	width(0),
	height(0),
	tiles(nullptr)
{
}

TileGrid::TileGrid(StringRef level_name)
	:
	width(0),
	height(0),
	tiles(nullptr)
{
	bool succes = LoadLevel(level_name);
	assert(succes && "failed to load the level");
}

TileGrid::TileGrid(const TileGrid & rhs)
	:
	width(rhs.width),
	height(rhs.height)
{
	tiles = new Tile[width * height];
	memcpy(tiles, rhs.tiles, width * height * sizeof(Tile));
}

TileGrid::TileGrid(TileGrid && rhs)
{
}

TileGrid & TileGrid::operator=(const TileGrid & rhs)
{
	// TODO: insert return statement here
}

TileGrid & TileGrid::operator=(TileGrid && rhs)
{
	// TODO: insert return statement here
}

TileGrid::~TileGrid()
{
}

bool TileGrid::LoadLevel(StringRef level_name)
{
	Serializer ser("");
	if (ser.Deserialize(level_name.data()) == ErrorCodes::FAILURE)
	{
		// failed to load the json file
		return false;
	}

	// get the width and height
	ser.Get("width", width);
	ser.Get("height", height);

	// allocate the tiles to fill the grid width
	tiles = new Tile[width * height];

}
