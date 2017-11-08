#pragma once

struct Cell;
class Grid;
class Triangle;
class Shader;

#include "VArray.h"

class Rasterizer
{
public:
  // rasterize cells
  template<typename Shader>
  void RasterizeCells(Grid& grid, Array<Shader>& commands);
};