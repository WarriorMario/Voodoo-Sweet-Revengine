#include "Scene.h"
#include "Keyboard.h"

Scene::Scene()
{
	frame_timer.Reset();
}

void Scene::Init(Graphics& gfx)
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD, simulation);
  simulation.CreateDebugDraw(gfx);
}

void Scene::Tick(Keyboard& kbd)
{
	float delta_time = frame_timer.Elapsed();

	// scope for updates
	{
		//simulation.Update();
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

void Scene::DebugDraw()
{
  simulation.DebugDraw();
}
