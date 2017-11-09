#include "Renderer.h"

Renderer::Renderer( Graphics & graphics)
  :
  gfx(graphics)
{

}

void Renderer::Render()
{
  // place in cells
  // rasterizer cells
  //  shade cells
  
	ApplyPasses(passes, grid , rasterizer);
  grid.UnPackBuffer(gfx.pSysBuffer);
  grid.Clear();
}
