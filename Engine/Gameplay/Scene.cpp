#include "Scene.h"
#include "Keyboard.h"

Scene::Scene()
  : players{nullptr}
{
}

void Scene::Init()
{
	tile_grid.LoadLevel(LEVEL_TO_LOAD);
}

void Scene::Tick(float dt, Keyboard& kbd)
{
  physx.Update();

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

Player* Scene::GetGod() const
{
  for(Player* player : players)
  {
    if(player != nullptr && player->is_god)
    {
      return player;
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

  int new_god = (rand() / (float)RAND_MAX) * 4;
  players[new_god]->is_god = true;
}
void Scene::SpawnPlayer(int idx, Vec2 pos)
{
  if(players[idx] != nullptr)
  {
    return;
  }

  players[idx] = new Player;
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