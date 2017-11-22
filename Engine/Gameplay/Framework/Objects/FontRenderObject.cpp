#include "FontRenderObject.h"
#include "Graphsicks\FontManager.h"

FontRenderObject::FontRenderObject()
  : TemplateBase("FontRenderObject"),
  start_x(300),
  start_y(300),
  orig_point_size(256),
  frames(0),
  max_size_diff(64)
{
  point_size = orig_point_size;
  // load a font
  auto& assetManager = Assets::Get();

  // load the font
  font = new Font("Fonts/times.ttf");

  // prerender the font so the font atlas is stored.
  font->RenderLine("Hello world with my crappy font rendering!", point_size, start_x, start_y);
}

void FontRenderObject::Update()
{
  //if(start_x < 1000)
  //{
  //  start_x += 1;
  //}
  //
  //if(start_y < 600)
  //{
  //  start_y += 1;
  //}

#if 1
  //point_size = sinf((frames / 100.0f) * PI) * max_size_diff + orig_point_size;
  frames++;
#endif
}

void FontRenderObject::Draw(Renderer & renderer)
{
  auto line = font->RenderLine("Hello world!", point_size, start_x, start_y);

  FontManager::RenderLine(line, renderer, Colors::White);
}
