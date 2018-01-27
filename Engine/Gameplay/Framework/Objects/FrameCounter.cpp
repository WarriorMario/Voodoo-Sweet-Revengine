#include "FrameCounter.h"
#include "Utility\DrawUtils.h"
#include "Assets\Assets.h"
#include "Assets\Font.h"
#include "Gameplay\Scene.h"

FrameCounter::FrameCounter(Scene& scene)
	:
	pos_x(10),
	pos_y(1050),
	point_size(32),
	frame_idx(0),
	has_filled_completely(false),
	scene(scene)
{
	// set to zero
	for (int i = 0; i < FRAMES_TO_MEASURE_OVER; ++i)
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
	if (frame_idx > FRAMES_TO_MEASURE_OVER)
	{
		has_filled_completely = true;
		frame_idx %= FRAMES_TO_MEASURE_OVER;
	}

	average_time = 0;
	for (int i = 0; i < FRAMES_TO_MEASURE_OVER; ++i)
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

	b2Vec2 pos = b2Vec2(scene.players[0]->x - Graphics::ScreenWidth / 2, 
		scene.players[0]->y + Graphics::ScreenHeight / 2 - 25);

	for (int i = 0; i < num_strings; ++i)
	{
		RenderText(renderer, text[i], *font, point_size, b2Vec2(pos.x, pos.y- i * 23), Colors::Red);
	}
}