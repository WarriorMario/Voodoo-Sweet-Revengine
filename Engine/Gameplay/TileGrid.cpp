#include "TileGrid.h"
#include "Tile.h"
#include "Utility\Serialization.h"
#include "Utility\Json\json.h"
#include "Assets\Assets.h"
#include "Assets\Texture.h"
#include "Graphsicks\Renderer.h"
#include <assert.h>

// small utility function to determine the tile's function
Tile::TileFunction GetFunction(int function)
{
	switch (function)
	{
	case 0:
		return Tile::TileFunction::COLLISION;
		break;
	case 2:
		return Tile::TileFunction::WATER;
		break;
	case 5:
		return Tile::TileFunction::WATER;
		break;
	default:
		return Tile::TileFunction::DEFAULT;
	}
}

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
	:
	width(rhs.width),
	height(rhs.height),
	tiles(rhs.tiles)
{
	rhs.width = 0;
	rhs.height = 0;
	rhs.tiles = nullptr;
}

TileGrid & TileGrid::operator=(const TileGrid & rhs)
{
	width = rhs.width;
	height = rhs.height;

	tiles = new Tile[width * height];
	memcpy(tiles, rhs.tiles, width * height * sizeof(Tile));

	return *this;
}

TileGrid & TileGrid::operator=(TileGrid && rhs)
{
	width = rhs.width;
	rhs.width = 0;

	height = rhs.height;
	rhs.height = 0;

	tiles = rhs.tiles;
	rhs.tiles = nullptr;

	return *this;
}

TileGrid::~TileGrid()
{
	if (tiles != nullptr)
	{
		delete[] tiles;
	}
}

bool TileGrid::LoadLevel(StringRef level_name)
{
	Texture* atlas = new Texture(TEXTURE_TO_LOAD);

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

	// get the tiles themselves
	ser.StepIn("layers");
	size_t counter = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++counter)
		{
			int visual;
			ser.StepInArray(0);
			ser.StepIn("data");
			ser.GetAtIdx(counter, visual);
			ser.StepOut();
			ser.StepOut();

			int function;
			ser.StepInArray(0);
			ser.StepIn("data");
			ser.GetAtIdx(counter, function);
			ser.StepOut();
			ser.StepOut();

			// -1 because for some reason, Tiled starts at 1 and not at 0
			visual -= 1; 
			function -= 1;
			tiles[counter].Init(x, y, visual, GetFunction(function), atlas);
		}
	}
}

void TileGrid::Draw(Renderer & renderer)
{
	size_t counter = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++counter)
		{
			tiles[counter].Render(renderer);
		}
	}
}
