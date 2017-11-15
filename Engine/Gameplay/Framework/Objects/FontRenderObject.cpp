#include "FontRenderObject.h"
#include "Assets\Assets.h"
#include "Utility.h"
#include "Graphsicks\Renderer.h"

FontRenderObject::FontRenderObject()
  : TemplateBase("FontRenderObject"),
  start_x(10),
  start_y(10),
  orig_point_size(128),
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
  if(start_x < 1000)
  {
    start_x += 1;
  }

  if(start_y < 600)
  {
    start_y += 1;
  }

#if 1
  point_size = sinf((frames / 100.0f) * PI) * max_size_diff + orig_point_size;
  frames++;
#endif
}

void FontRenderObject::Draw(Renderer & renderer)
{
  auto line = font->RenderLine("Hello world!", point_size, start_x, start_y);
  
  for(int i = 0; i < line.num_quads; ++i)
  {
    UIShader shader;
    shader.const_data.color = Colors::White;
    shader.const_data.pixels = line.text_quads[i].glyph_pixels;
    shader.const_data.width = line.text_quads[i].glyph_width;
    shader.const_data.height = line.text_quads[i].glyph_height;

    float min_x = line.text_quads[i].min_x,
          min_y = line.text_quads[i].min_y,
          max_x = line.text_quads[i].max_x,
          max_y = line.text_quads[i].max_y;
    
    float min_u = line.text_quads[i].min_u,
          min_v = line.text_quads[i].min_v,
          max_u = line.text_quads[i].max_u,
          max_v = line.text_quads[i].max_v;

    // first triangle      x      y      u      v
    shader.prim_data[0] = {min_x, min_y, min_u, min_v};
    shader.prim_data[1] = {max_x, min_y, max_u, min_v};
    shader.prim_data[2] = {min_x, max_y, min_u, max_v};

    renderer.AddDrawCommand<UIShader>(shader);

    // second triangle
    shader.prim_data[0] = {max_x, min_y, max_u, min_v};
    shader.prim_data[1] = {max_x, max_y, max_u, max_v};
    shader.prim_data[2] = {min_x, max_y, min_u, max_v};

    renderer.AddDrawCommand<UIShader>(shader);


    // left-top tri
    // pcs
    //x -
    //x +
    //x -
    //y -
    //y -
    //y +

    // bot-right tri
    // pcs
    //x +
    //x +
    //x -
    //y -
    //y +
    //y +
  }
}
