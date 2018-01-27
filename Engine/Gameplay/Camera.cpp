#include "Camera.h"
#include "Player.h"

void Camera::CalculateOffset(const Array<Player*>& players)
{
  Vec2 average = Vec2(0,0);
  size_t i = 0;
  for(; i < players.size(); ++i)
  {
    average += {players[i]->x, players[i]->y};
  }
  offset = -average / i
    + Vec2(Graphics::ScreenWidth/2, Graphics::ScreenHeight/ 2);
}
