#include "Renderer.h"

Renderer::Passes<BackgroundShader, ForegroundShader, UIShader, UIShaderSimple> Renderer::shared_passes;

Renderer::Renderer()
{
}

void Renderer::AdjustCamera(const Array<Player*>& players)
{
  camera.CalculateOffset(players);
}
void Renderer::Render(Color* target)
{
  rasterizer.FlipCheckerBoard();
  ForEach(shared_passes, PassApplyer(), grid, rasterizer, camera);
  ForEach(passes, PassApplyer(), grid, rasterizer, camera);
	
  grid.UnPackBuffer(target);
}
void Renderer::Reset()
{
  ForEach(shared_passes, PassClearer());
  ForEach(passes, PassClearer());
  grid.Clear();
}
