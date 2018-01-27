#include "Tile.h"
#include "Assets\Assets.h"
#include "Assets\Texture.h"
#include "Utility\DrawUtils.h"
#include "Physics\Physics.h"

Tile::Tile()
{
}

void Tile::Init(int grid_pos_x, int grid_pos_y, int tile_visual,
	TileFunction tile_function, Texture* atlas_texture, Physics& simulation)
{
	// transform the position to screen space
	pos_x = grid_pos_x * SIZE;
	pos_y = grid_pos_y * SIZE;

	// save a copy of the atlas and type
	atlas = atlas_texture;
	visual = tile_visual;
	function = tile_function;

	// get the texture width and height of the atlas
	int tex_width = atlas->GetWidth(), tex_height = atlas->GetHeight();

	// number of tiles in atlas per axis
	int atlas_width = tex_width / TILE_SIZE_IN_ATLAS;
	int atlas_height = tex_height / TILE_SIZE_IN_ATLAS;
	
	// the step size in u and v per tile
	float uv_delta_x = (float)TILE_SIZE_IN_ATLAS / (float)tex_width;
	float uv_delta_y = (float)TILE_SIZE_IN_ATLAS / (float)tex_height;

	// my position in the atlas
	int atlas_x = visual % atlas_width;
	int atlas_y = visual / atlas_width;

	// calculate my UV coordinates
	min_u = atlas_x * uv_delta_x + 0.001f;
	max_u = min_u + uv_delta_x - 0.002f;
	min_v = atlas_y * uv_delta_y + 0.001f;
	max_v = min_v + uv_delta_y - 0.002f;
  InitFunction(tile_function,simulation);
}

void Tile::InitFunction(TileFunction function, Physics& simulation)
{
  switch(function)
  {
    case Tile::DEFAULT:
    break;
    case Tile::COLLISION:
      simulation.CreateBody(Vec2(pos_x/PHYSICS_SCALE,pos_y / PHYSICS_SCALE), "Square", BodyType::STATIC);
      break;
    case Tile::BORDER:
    {
      Body & body = simulation.CreateBody(Vec2(pos_x / PHYSICS_SCALE, pos_y / PHYSICS_SCALE), "Square", BodyType::STATIC);
      body.body->SetUserData((void*)Tile::BORDER);
    }
      break;
    case Tile::WATER:
    {
      Body& body = simulation.CreateBody(Vec2(pos_x / PHYSICS_SCALE, pos_y / PHYSICS_SCALE), "Square", BodyType::STATIC);
      body.body->SetUserData((void*)Tile::WATER);
    }
    break;
    default:
      break;
  }
}

void Tile::Render()
{
	RenderSharedQuad<ForegroundShader>(b2Vec2(pos_x, pos_y), b2Vec2(SIZE, SIZE),
		b2Vec2(min_u, min_v), b2Vec2(max_u, max_v), false, true, Colors::Cyan, atlas);
}
