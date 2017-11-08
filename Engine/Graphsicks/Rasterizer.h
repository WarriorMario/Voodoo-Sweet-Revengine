#pragma once

struct Cell;
class Grid;
class Triangle;
class Shader;

class Rasterizer
{
public:
  // rasterize cells
  void RasterizeCells(Grid& grid);
};