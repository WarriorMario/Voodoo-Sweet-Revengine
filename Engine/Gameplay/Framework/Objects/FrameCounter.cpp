#include "FrameCounter.h"
#include "Assets\Assets.h"
#include "Utility.h"
#include "Graphsicks\Renderer.h"
#include "VString.h"

FrameCounter::FrameCounter():
  pos_x(10),
  pos_y(1050),
  point_size(32),
  frame_idx(0),
  has_filled_completely(false)
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
  average_fps = 0;
}

void FrameCounter::Update()
{
  // update times stored
  this_frame_time = timer.Elapsed();
  past_frame_times[frame_idx] = this_frame_time;
  frame_idx++;
  if(frame_idx > FRAMES_TO_MEASURE_OVER)
  {
    has_filled_completely = true;
    frame_idx %= FRAMES_TO_MEASURE_OVER;
  }

  average_time = 0;
  for(int i = 0; i < FRAMES_TO_MEASURE_OVER; ++i)
  {
    average_time += past_frame_times[i];
  }
  average_time *= INV_FRAMES_TO_MEASURE_OVER;
  average_fps = int(1.0 / average_time);
  fps = 1.0 / this_frame_time;
  timer.Reset();
}

void FrameCounter::Draw(Renderer & renderer)
{
  static constexpr size_t num_strings = 6;
  String text[num_strings];
  text[0] = "Frames per Second: " + std::to_string(fps);
  text[1] = "Seconds per Frame: " + std::to_string(this_frame_time);
  text[2] = "Average Frames per Second: " + std::to_string(average_fps);
  text[3] = "Average Seconds per Frame: " + std::to_string(average_time);
  text[4] = "Averaged over " + std::to_string(FRAMES_TO_MEASURE_OVER) + " frames.";
  text[5] = has_filled_completely ? "The average is reliable" : "The average is still being calculated...";

  for(int i = 0; i < num_strings; ++i)
  {
    auto line = font->RenderLine(text[i], point_size, pos_x, pos_y - i * 23);

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
}