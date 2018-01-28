#include "Scene.h"
#include "Keyboard.h"
#include "AngryPlayer.h"
#include "Physics\Physics.h"

Scene::Scene()
	: players{ nullptr }
{
}

void Scene::Init(Graphics& gfx)
{
  simulation.CreateDebugDraw(gfx);
	tile_grid.LoadLevel(LEVEL_TO_LOAD, simulation);
}

void Scene::Tick(float dt, Input& kbd)
{
	simulation.Update();

	for (Player* player : players)
	{
		if (player != nullptr)
		{
			player->Update(dt);
			player->Input(kbd);
		}
	}
}

void Scene::Draw()
{
	tile_grid.Draw();
	for (Player* player : players)
	{
		if (player != nullptr) player->Draw();
	}
}

AngryPlayer* Scene::GetGod() const
{
	for (Player* player : players)
	{
		if (player != nullptr && player->is_god)
		{
			return (AngryPlayer*)player;
		}
	}
	return nullptr;
}
Array<Player*> Scene::GetPlayers() const
{
	Array<Player*> result;
	for (Player* player : players)
	{
		if (player != nullptr && !player->is_god)
		{
			result.push_back(player);
		}
	}
	return result;
}

void Scene::NewGod()
{
	for (Player* player : players)
	{
		if (player != nullptr)
		{
			player->is_god = false;
		}
	}

  int new_god = NUM_PLAYERS - 1;;
	if (players[new_god] != nullptr)
	{
		delete players[new_god];
	}
	players[new_god] = new AngryPlayer(simulation, tile_grid, new_god);
	players[new_god]->is_god = true;

	Vec2 pos = tile_grid.GetRandomSpawnGod();
	players[new_god]->x = pos.x;
	players[new_god]->y = pos.y;
}

void Scene::SpawnPlayer(int idx)
{
	if (players[idx] != nullptr)
	{
		return;
	}

	players[idx] = new Player(simulation, tile_grid, idx);

	Vec2 pos = tile_grid.GetRandomSpawnPlayer();
	players[idx]->x = pos.x;
	players[idx]->y = pos.y;

}

void Scene::DebugDraw()
{
  simulation.DebugDraw();
}

void Scene::KillPlayer(int idx)
{
	if (players[idx] == nullptr)
	{
		return;
	}

	delete players[idx];
}