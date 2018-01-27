#include "Scene.h"
#include "Keyboard.h"
#include "AngryPlayer.h"

Scene::Scene()
  : players{nullptr}
{
}

void Scene::Init()
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD,simulation);
}

void Scene::Tick(float dt, Input& kbd)
{
  simulation.Update();

  for(Player* player : players)
  {
    if(player != nullptr)
    {
      player->Update();
      player->Input(kbd);
    }
  }
}

void Scene::Draw()
{
  tile_grid.Draw();
  for(Player* player : players)
  {
    if(player != nullptr) player->Draw();
  }
}

AngryPlayer* Scene::GetGod() const
{
  for(Player* player : players)
  {
    if(player != nullptr && player->is_god)
    {
      return (AngryPlayer*)player;
    }
  }
}
Array<Player*> Scene::GetPlayers() const
{
  Array<Player*> result;
  for(Player* player : players)
  {
    if(player != nullptr && !player->is_god)
    {
      result.push_back(player);
    }
  }
  return result;
}

void Scene::NewGod()
{
  for(Player* player : players)
  {
    if(player != nullptr) player->is_god = false;
  }

  int new_god = 3;
  delete players[new_god];
  players[new_god] = new AngryPlayer(simulation, tile_grid, 3);
  players[new_god]->is_god = true;
}
void Scene::SpawnPlayer(int idx, Vec2 pos)
{
  if(players[idx] != nullptr)
  {
    return;
  }

  players[idx] = new Player(simulation,tile_grid,idx);
  players[idx]->x = pos.x;
  players[idx]->y = pos.y;

}
void Scene::KillPlayer(int idx)
{
  if(players[idx] == nullptr)
  {
    return;
  }

  delete players[idx];
}