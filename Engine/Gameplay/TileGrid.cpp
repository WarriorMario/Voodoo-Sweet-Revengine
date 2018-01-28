#include "TileGrid.h"
#include "Tile.h"
#include "Utility\Serialization.h"
#include "Utility\Json\json.h"
#include "Assets\Assets.h"
#include "Assets\Texture.h"
#include "Graphsicks\Renderer.h"
#include "Physics\Physics.h"
#include <assert.h>
#include <random>

// small utility function to determine the tile's function
Tile::TileFunction GetFunction(int function)
{
	switch (function)
	{
	case 0:
		return Tile::TileFunction::COLLISION;
	case 2:
		return Tile::TileFunction::BORDER_SOURCE;
	case 3:
		return Tile::TileFunction::BORDER_GOAL;
	case 4:
		return Tile::TileFunction::GOD_SPAWN_LOCATION;
	case 5:
		return Tile::TileFunction::PLAYER_SPAWN_LOCATION;
	case 6:
		return Tile::TileFunction::WATER_SOURCE;
	case 7:
		return Tile::TileFunction::WATER_GOAL;
	case -668:
		return Tile::TileFunction::DEFAULT;
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

TileGrid::TileGrid(StringRef level_name, Physics& simulation)
	:
	width(0),
	height(0),
	tiles(nullptr)
{
	bool succes = LoadLevel(level_name, simulation);
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

bool TileGrid::LoadLevel(StringRef level_name, Physics& simulation)
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

	// get the offsets
	int offset_visual;
	int offset_function;
	ser.StepIn("tilesets");
	ser.StepInArray(0);
	ser.Get("firstgid", offset_visual);
	ser.StepOut();
	ser.StepInArray(1);
	ser.Get("firstgid", offset_function);
	ser.StepOut();
	ser.StepOut();

	// get the tiles themselves
	ser.StepIn("layers");
	size_t counter = 0;
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x, ++counter)
		{
			int visual;
			ser.StepInArray(0);
			ser.StepIn("data");
			ser.GetAtIdx(counter, visual);
			ser.StepOut();
			ser.StepOut();

			int function_value;
			ser.StepInArray(1);
			ser.StepIn("data");
			ser.GetAtIdx(counter, function_value);
			ser.StepOut();
			ser.StepOut();

			function_value -= offset_function; // because for some reason they're offset with this
			Tile::TileFunction function = GetFunction(function_value);

			// init the tile
			tiles[counter].Init(x, y, visual - offset_visual, function, atlas, simulation);

			// add any possible spawn locations
			if (function == Tile::TileFunction::PLAYER_SPAWN_LOCATION)
			{
				player_spawn_locations.push_back(tiles[counter].GetPos() + Vec2(Tile::SIZE / 2, Tile::SIZE / 2));
			}
			else if (function == Tile::TileFunction::GOD_SPAWN_LOCATION)
			{
				god_spawn_locations.push_back(tiles[counter].GetPos() + Vec2(Tile::SIZE / 2, Tile::SIZE / 2));
			}
		}
	}
}

void TileGrid::Draw()
{
	size_t counter = 0;
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x, ++counter)
		{
			tiles[counter].Render();
		}
	}
}

bool TileGrid::IsPassable(int x, int y, bool is_neighbour)
{
	return tiles[y*width + x].IsPassable(is_neighbour);
}

bool TileGrid::DrinkingArea(int x, int y)
{
  return tiles[y*width + x].DrinkingArea();
}

b2Vec2 TileGrid::GetRandomSpawnPlayer()
{
	assert(player_spawn_locations.size() > 0 && "There are no spawn locations in this map, or something went wrong with loading.");
	int randIdx = std::rand() % player_spawn_locations.size();
	return player_spawn_locations[randIdx];
}

b2Vec2 TileGrid::GetRandomSpawnGod()
{
	assert(god_spawn_locations.size() > 0 && "There are no spawn locations in this map, or something went wrong with loading.");
	int randIdx = std::rand() % god_spawn_locations.size();
	return god_spawn_locations[randIdx];
}
