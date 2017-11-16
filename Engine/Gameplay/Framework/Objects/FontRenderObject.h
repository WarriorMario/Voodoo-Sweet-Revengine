#pragma once
#include "ArenaObject.h"

class Font;

class FontRenderObject : public ArenaObject<FontRenderObject>
{
public:
  FontRenderObject();


  void Update();
  void Draw(class Renderer& renderer);


  void SetPosition(struct Vec2& pos);
private:
  Font* font;
  int start_x, start_y;
  int point_size;
  int orig_point_size;
  int frames;
  int max_size_diff;
};