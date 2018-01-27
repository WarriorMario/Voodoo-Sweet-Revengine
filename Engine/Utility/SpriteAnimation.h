#pragma once

#include "Assets/Assets.h"
#include "Matrix.h"
#include "VArray.h"

class SpriteAnimation
{
public:
  SpriteAnimation(StringRef clipname, int frame_count, float delay);

  void Update(float dt);
  void Draw(Vec2 pos, Vec2 size, bool flip_x, bool flip_y);

private:
  const float delay;
  const int frame_count;
  
  int curr_frame;
  float elapsed;

  Array<Texture*> frames;
};