#pragma once
#include "ArenaObject.h"
#include "Utility\Timer.h"

class Font;
static constexpr int FRAMES_TO_MEASURE_OVER = 15;
static constexpr double INV_FRAMES_TO_MEASURE_OVER = 1.0 / (double)FRAMES_TO_MEASURE_OVER;

class FrameCounter : public ArenaObject<FrameCounter>
{
public:
  FrameCounter();


  void Update();
  void Draw(class Renderer& renderer);

private:
  Font* font;
  int pos_x, pos_y;
  int point_size;
  int frame_idx;
  double this_frame_time;
  int fps;
  double past_frame_times[FRAMES_TO_MEASURE_OVER];
  double average_time;
  Timer timer;
};