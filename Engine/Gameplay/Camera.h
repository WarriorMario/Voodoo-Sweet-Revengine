#pragma once
#include "Matrix.h"
#include "VArray.h"
#include "Graphsicks\Graphics.h"

class Player;

class Camera
{
public:
  void CalculateOffset(const Array<Player*>& players);
  Vec2 Transform(Vec3 vector)
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
  Vec2 offset = Vec2(0.0f, 0.0f);
  float zoom = 0.5f;
  //float angle = 0.0f;
};