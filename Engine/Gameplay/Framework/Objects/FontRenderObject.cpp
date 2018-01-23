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
}

void FontRenderObject::Draw(Renderer & renderer)
{
  //auto line = font->RenderLine("Hello world!", point_size, start_x, start_y);
  //
  //FontManager::RenderLine(line, renderer, Colors::White);
	RenderLinePerGlyph("Hello world!", point_size, start_x, start_y, *font, renderer, Colors::White);
}

void FontRenderObject::SetPosition(Vec2 & pos)
{
  start_x = pos.x;
  start_y = pos.y;
}
