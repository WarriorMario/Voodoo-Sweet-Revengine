#include "Camera.h"
#include "Player.h"

void Camera::CalculateOffset(const Array<Player*>& players)
{
	float biggest_axis = 0.0f;

  Vec2 average = Vec2(0,0);
  float minX = FLT_MAX, maxX = FLT_MIN;
  float minY = FLT_MAX, maxY = FLT_MIN;

  size_t i = 0;
  for(; i < players.size(); ++i)
  {
    average += {players[i]->x, players[i]->y};

    minX = min(players[i]->x, minX);
    maxX = max(players[i]->x, maxX);
    minY = min(players[i]->y, minY);
    maxY = max(players[i]->y, maxY);
  }
  offset = -average / i
    + Vec2(Graphics::ScreenWidth/2, Graphics::ScreenHeight/ 2);

  const float minZoom = 0.5f;
  const float deltaX = max(abs(maxX - minX), minZoom * Graphics::ScreenWidth);
  const float deltaY = max(abs(maxY - minY), minZoom * Graphics::ScreenHeight);

  const float scaleMargin = 400.f;
  const float scaleX = deltaX / (Graphics::ScreenWidth - scaleMargin);
  const float scaleY = deltaY / (Graphics::ScreenHeight - scaleMargin);

  zoom = max(scaleX, scaleY);
}

Vec2 Camera::Transform(Vec3 vector)
{
	Vec2 screenCorrection = Vec2(
		Graphics::ScreenWidth / 2,
		Graphics::ScreenHeight / 2);

	const Vec2 invOffset = Vec2(-offset.x, -offset.y);

	const float invZoom = 1.0f / zoom;
	//const float invAngle = -angle;
	//
	Mat2 scale(invZoom, invZoom);
	//b2Rot rotation(-invAngle);

	Vec2 res = Vec2(vector.x, vector.y);
	res *= invZoom;
	Vec2 temp_offset = offset * invZoom;

	res += temp_offset;

	res += screenCorrection * (1.0f - invZoom);


	return Vec2(res.x, res.y);
}
