#pragma once
#include "Rasterizer.h"
#include "Graphics.h"
#include "ScreenGrid.h"
class Graphics;

class Renderer
{
public:
  Renderer(Graphics& gfx);
  void UnPackToBuffers();
public:
  Rasterizer rasterizer;
  Graphics& gfx;
  ScreenGrid<60, 135, 32, 8> grid1920_1080p;
  //Grid<60, 34, 32, 32> grid1920_1088p_1;
  //Grid<30, 17, 64, 64> grid1920_1088p_2;
  //Grid<15, 17, 128, 64> grid1920_1088p_3;
};