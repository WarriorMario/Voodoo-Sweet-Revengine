#include "FrameCounter.h"
#include "Assets\Assets.h"
#include "Utility.h"
#include "Graphsicks\Renderer.h"
#include "VString.h"

FrameCounter::FrameCounter():
  pos_x(10),
  pos_y(1000),
  point_size(64),
  frame_idx(0)
{
  // set to zero
  for(int i = 0; i < FRAMES_TO_MEASURE_OVER; ++i)
  {
    past_frame_times[i] = 0.0f;
  }
  // load a font
  auto& assetManager = Assets::Get();

  // load the font
  font = new Font("Fonts/comic.ttf");

  // prerender the font so the font atlas is stored.
  font->RenderLine("Plz load for me", point_size, pos_x, pos_y);

  // start timing
  timer.Reset();
  fps = 0;
}

void FrameCounter::Update()
{
  // update times stored
  this_frame_time = timer.Elapsed();
  past_frame_times[frame_idx] = this_frame_time;
  frame_idx++;
  frame_idx %= FRAMES_TO_MEASURE_OVER;

  average_time = 0;
  for(int i = 0; i < FRAMES_TO_MEASURE_OVER; ++i)
  {
    average_time += past_frame_times[i];
  }
  average_time *= INV_FRAMES_TO_MEASURE_OVER;
  fps = int(1.0 / average_time);

  timer.Reset();
}

void FrameCounter::Draw(Renderer & renderer)
{
  String text[2];
  text[0] = "FPS on average : " + std::to_string(fps);
  text[1] = "Time this frame: " + std::to_string(this_frame_time);
  for(int i = 0; i < 2; ++i)
  {
    auto line = font->RenderLine(text[i], point_size, pos_x, pos_y - i * 50);

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
    }
  }
}