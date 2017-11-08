#pragma once
class Rasterizer;
class Graphics;

class Renderer
{
public:
  Rasterizer& rasterizer;
  union
  {
    Graphics& graphsicks;
    Graphics& gfx;
    Graphics& graphics;
  };
};