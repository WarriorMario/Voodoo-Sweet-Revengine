#include "AngryPlayer.h"
#include "Physics\Physics.h"
#include "Tile.h"
#define IDLE_LAYER_0 "Images/Neighbour_anim/Layer0/0_BadGuyWalk"

#define MOVE_LAYER_0 "Images/Neighbour_anim/Layer0/0_BadGuy"
#define MOVE_LAYER_1 "Images/Neighbour_anim/Layer0/0_BadGuy_2"
#define MOVE_LAYER_2 "Images/Neighbour_anim/Layer0/0_BadGuy_3"
#define MOVE_LAYER_3 "Images/Neighbour_anim/Layer0/0_BadGuy_4"

#define SMASH_LAYER_0 "Images/Kid1_Anim/Layer0/0_BadGuySmash"
#define SMASH_LAYER_1 "Images/Kid1_Anim/Layer1/0_BadGuySmash_2"
#define SMASH_LAYER_2 "Images/Kid1_Anim/Layer2/0_BadGuySmash_3"
#define SMASH_LAYER_3 "Images/Kid1_Anim/Layer3/0_BadGuySmash_4"

AngryPlayer::AngryPlayer(Physics & simulation, TileGrid & grid, int id)
  :
  Player(simulation,grid)
{
  player_id = id;
  width = 64;
  height = 64;
  physics_body.body->SetTransform(Vec2(64, 24), 0);
  x = 640;
  y = 240;
  speed = GodBaseSpeed;




  graphics[(int)Sprite::Idle].AddLayer(IDLE_LAYER_0, 1);

  graphics[(int)Sprite::Move].AddLayer(MOVE_LAYER_0, 4);

  graphics[(int)Sprite::Smash].AddLayer(SMASH_LAYER_0, 4);


  graphics[(int)Sprite::Idle].ScaleLayer(0, BaseScale);
  graphics[(int)Sprite::Move].ScaleLayer(0, BaseScale);
  graphics[(int)Sprite::Smash].ScaleLayer(0, BaseScale);

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
      if((size_t)player > Tile::NUM_FUNCTIONS)
      {
        player->LoseWater();
      }
    }
    contacts = contacts->next;
  }
}
