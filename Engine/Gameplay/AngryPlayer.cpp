#include "AngryPlayer.h"
#include "Physics\Physics.h"
#include "Tile.h"
AngryPlayer::AngryPlayer(Physics & simulation, TileGrid & grid, int id)
  :
  Player(simulation,grid,id)
{
  width = 64;
  height = 64;
  physics_body.body->SetTransform(Vec2(23, 19), 0);
  x = 230;
  y = 190;
}

void AngryPlayer::Update(float dt)
{
  Player::Update(dt);

  // Check overlaps with other players
  b2ContactEdge* contacts = physics_body.body->GetContactList();
  while(contacts)
  {
    if(contacts->contact->IsTouching())
    {
      Player* player = (Player*)contacts->contact->GetFixtureA()->GetBody()->GetUserData();
      if((size_t)player > Tile::WATER)
      {
        player->LoseWater();
      }
    }
    contacts = contacts->next;
  }
}
