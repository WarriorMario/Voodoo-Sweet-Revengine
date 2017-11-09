#include "Renderer.h"

Renderer::Renderer(Graphics & graphics)
	:
	gfx(graphics)
{

}

void Renderer::Render()
{
	// place in cells
	// rasterizer cells
	//  shade cells

	ApplyPasses(passes, grid, rasterizer);
	grid.UnPackBuffer(gfx.god_window_buffer);
	grid.Clear();
}
