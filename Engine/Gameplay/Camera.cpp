#include "Camera.h"
#include "Player.h"

void Camera::CalculateOffset(Player ** players, size_t num_players)
{
  Vec2 average = Vec2(0,0);
  size_t i = 0;
  for(; i < num_players; ++i)
  {
    if(players[i] == nullptr)
    {
      break;
    }
    average += {players[i]->x, players[i]->y};
  }
  offset = -average / i + Vec2(Graphics::ScreenWidth/2, Graphics::ScreenHeight/ 2);
}
