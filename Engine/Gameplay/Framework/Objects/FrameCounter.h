#pragma once
#include "ArenaObject.h"
#include "Utility\Timer.h"

class Font;
class Scene;
static constexpr int FRAMES_TO_MEASURE_OVER = 1000;
static constexpr double INV_FRAMES_TO_MEASURE_OVER = 1.0 / (double)FRAMES_TO_MEASURE_OVER;

class FrameCounter
{
public:
	FrameCounter();

	void Update(float dt);
	void Draw(class Renderer& renderer);

private:
	int pos_x, pos_y;
	int point_size;
	int frame_idx;
	float this_frame_time;
	int average_fps;
	int fps;
	float past_frame_times[FRAMES_TO_MEASURE_OVER];
	float average_time;
	bool has_filled_completely;
};