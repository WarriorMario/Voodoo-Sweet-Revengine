#pragma once
#include "Matrix.h"

class Triangle
{
public:
  Triangle()
  {
    for(int i = 0; i < 3; ++i)
    {
      p[i] = Vec2(0,0);
      uv[i] = Vec2(0,0);
    }
  }
  Triangle(Vec2& p0, Vec2& p1, Vec2& p2, Vec2& uv0, Vec2& uv1, Vec2& uv2)
  {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    uv[0] = uv0;
    uv[1] = uv1;
    uv[2] = uv2;
  }

  Vec2 p[3];
  Vec2 uv[3];
};