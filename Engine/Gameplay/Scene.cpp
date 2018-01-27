#include "Scene.h"
#include "Keyboard.h"
#include "AngryPlayer.h"

Scene::Scene()
{
	frame_timer.Reset();
}

void Scene::Init(Graphics& gfx)
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD, simulation);
  simulation.CreateDebugDraw(gfx);
  for(int i = 0; i < NUM_PLAYERS-1; ++i)
  {
    players[i] = new Player(simulation,tile_grid,i);
    players[i]->x += i * 100;
  }
  players[NUM_PLAYERS - 1] = new AngryPlayer(simulation, tile_grid, 3);

}

void Scene::Tick(Input& kbd)
{
	float delta_time = frame_timer.Elapsed();

	// scope for updates
	{
		simulation.Update();
    for(int i = 0; i < NUM_PLAYERS; ++i)
    {
		  players[i]->Input(kbd);
		  players[i]->Update();
    }
	}
	frame_timer.Reset();
}

void Scene::Draw(Renderer & renderer)
{
	// Update players camera
	renderer.camera.CalculateOffset(players, NUM_PLAYERS);

	tile_grid.Draw(renderer);
  for(int i = 0; i < NUM_PLAYERS; ++i)
  {
	  players[i]->Draw(renderer);
  }
}

void Scene::DebugDraw()
{
  simulation.DebugDraw();
}
