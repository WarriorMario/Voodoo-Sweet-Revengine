#include "Renderer.h"
Renderer::Renderer(Graphics & graphics)
	:
	gfx(graphics)
{
  camera.offset = Vec2(1000, 0);
}

void Renderer::Render()
{
  rasterizer.FlipCheckerBoard();
  ForEach(passes, PassApplyer(), grid, rasterizer, camera);
	grid.UnPackBuffer(gfx.god_window_buffer);
  grid.UnPackBuffer(gfx.player_window_buffer);
	grid.Clear();
}
