#include "Scene.h"

Scene::Scene()
{

}

void Scene::Init()
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD);
}

void Scene::Tick(float delta_time)
{
	
}

void Scene::Draw(Renderer & renderer)
{
	tile_grid.Draw(renderer);
}
