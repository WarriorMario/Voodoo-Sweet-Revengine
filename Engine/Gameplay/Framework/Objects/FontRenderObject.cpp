#include "FontRenderObject.h"
#include "Assets\Assets.h"
#include "Utility.h"
#include "Graphsicks\Renderer.h"

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
  auto line = font->RenderLine("Hello world!", point_size, start_x, start_y);

  for(int i = 0; i < line.num_quads; ++i)
  {
    float min_x = line.text_quads[i].min_x,
      min_y = line.text_quads[i].min_y,
      max_x = line.text_quads[i].max_x,
      max_y = line.text_quads[i].max_y;

    float min_u = line.text_quads[i].min_u,
      min_v = line.text_quads[i].min_v,
      max_u = line.text_quads[i].max_u,
      max_v = line.text_quads[i].max_v;


    // first triangle      x      y      u      v
    UIShader::PrimData vertices[3];
    vertices[0] = {min_x, min_y, min_u, min_v};
    vertices[1] = {max_x, min_y, max_u, min_v};
    vertices[2] = {min_x, max_y, min_u, max_v};

    UIShader shader = UIShader(vertices[0], vertices[1], vertices[2],
      line.text_quads[i].glyph_alpha_values,
      line.text_quads[i].glyph_width, line.text_quads[i].glyph_height,
      Colors::Red);

    renderer.AddDrawCommand<UIShader>(shader);

    // second triangle
    vertices[0] = {max_x, min_y, max_u, min_v};
    vertices[1] = {max_x, max_y, max_u, max_v};
    vertices[2] = {min_x, max_y, min_u, max_v};

    shader = UIShader(vertices[0], vertices[1], vertices[2],
      line.text_quads[i].glyph_alpha_values,
      line.text_quads[i].glyph_width, line.text_quads[i].glyph_height,
      Colors::Red);

    renderer.AddDrawCommand<UIShader>(shader);
  }
}

void FontRenderObject::SetPosition(Vec2 & pos)
{
  start_x = pos.x;
  start_y = pos.y;
}
