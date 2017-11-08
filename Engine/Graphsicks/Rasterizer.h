#pragma once
#include "Triangle.h"
#include "Shader.h"

struct ScreenGridCell;
class Triangle;

class Rasterizer
{
public:
  // rasterize cells
  void RasterizeCells(ScreenGridCell* cells,
    Triangle* tri_buff, Shader shader,
    int cell_width, int cell_height,
    int grid_width, int grid_height);

private:
  // rasterize a single cell. Utility function for public RasterizeCells
  void RasterizeCell(ScreenGridCell& cell,
    Triangle* tri_buff, Shader shader,
    int cell_x, int cell_y,
    int cell_width, int cell_height);

  // see if the point is to the right of the line. Utility function for RasterizeCell
  bool IsRightOfLine(const Vec2& v1, const Vec2& v2, const Vec2& p);
};