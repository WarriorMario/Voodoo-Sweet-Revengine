#include "Scene.h"
#include "Keyboard.h"

Scene::Scene()
{
	frame_timer.Reset();
}

void Scene::Init()
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD);
}

void Scene::Tick(Keyboard& kbd)
{
	float delta_time = frame_timer.Elapsed();

	// scope for updates
	{
		player.Update();
		player.Input(kbd);
	}

	frame_timer.Reset();
}

void Scene::Draw(Renderer & renderer)
{
	Player* players[] = { &player };
	// Update players camera
	renderer.camera.CalculateOffset(players, 1);

	tile_grid.Draw(renderer);
	player.Draw(renderer);
}
