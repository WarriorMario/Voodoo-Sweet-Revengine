#pragma once

#include "Assets/Assets.h"
#include "Matrix.h"
#include "VArray.h"

// ****************************************************************************

class SpriteAnimation
{
public:
  SpriteAnimation(StringRef clipname, int frame_count, float delay);
  ~SpriteAnimation();

  void Update(float dt);
  void Draw(Vec2 pos, Vec2 size, bool flip_x, bool flip_y);

private:
  const float delay;
  const int frame_count;
  
  int curr_frame;
  float elapsed;

  Array<Texture> frames;
};

// ****************************************************************************

class LayeredAnimation
{
public:
  LayeredAnimation(float delay) :
    delay(delay)
  {}

  void Update(float dt);
  void Draw(Vec2 pos, Vec2 size, bool flip_x, bool flip_y);

  void AddLayer(StringRef clipname, int frame_count)
  {
    layers.emplace_back(clipname, frame_count, delay);
  }
  void ScaleLayer(int idx, float scale)
  {
    layers[idx].scale = scale;
  }
  void OffsetLayer(int idx, Vec2 offset)
  {
    layers[idx].offset = offset;
  }

private:
  const float delay;

  struct Layer
  {
    Layer(StringRef clip_name, int frame_count, float delay) :
      animation(clip_name, frame_count, delay),
      scale(1.f),
      offset(0.f)
    {}

    SpriteAnimation animation;
    float scale;
    Vec2 offset;
  };
  Array<Layer> layers;

};