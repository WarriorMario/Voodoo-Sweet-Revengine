#include "VArray.h"
#include "ScreenGrid.h"
#include "Graphics.h"

class Rasterizer
{
public:
  // rasterize cells
  //void RasterizeCells(ScreenGridCell* cells,
  //  Triangle* tri_buff, Shader shader,
  //  int cell_width, int cell_height,
  //  int grid_width, int grid_height);
  template<typename Shader>
  void RasterizeCells(ScreenGrid& grid, Array<Shader>& commands)
  {
    for(int y = 0; y < ScreenGrid::HEIGHT; ++y)
    {
      for(int x = 0; x < ScreenGrid::WIDTH; ++x)
      {
        RasterizeCell(grid, grid.cells[y * ScreenGrid::WIDTH + x], commands,
          x, y);
      }
    }
  }
private:

  template<typename Shader>
  void RasterizeCell(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;

    for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
    {
      for(int x = 0; x < ScreenGrid::CELL_WIDTH; ++x)
      {
        for(int j = 0; j < cell.num_indices; ++j)
        {
          auto& cmd = commands[cell.indices[j]];

          Vec2 p = Vec2((float)(x + start_x), (float)(y + start_y));
          auto* d = cmd.prim_data;

          const float t0 = (p.x - d[1].x) * (d[2].y - d[1].y) - (p.y - d[1].y) * (d[2].x - d[1].x);
          const float t1 = (p.x - d[2].x) * (d[0].y - d[2].y) - (p.y - d[2].y) * (d[0].x - d[2].x);
          const float t2 = (p.x - d[0].x) * (d[1].y - d[0].y) - (p.y - d[0].y) * (d[1].x - d[0].x);
          const bool is_inside = (t0 <= 0) & (t1 <= 0) & (t2 <= 0);
          if(is_inside == true)
          {
            const Vec2 A(d[1].x - d[0].x, d[1].y - d[0].y);
            const Vec2 B(d[2].x - d[0].x, d[2].y - d[0].y);
            const float prarea = -1.f / (A.x * B.y - A.y * B.x);

            // Determine interpolated values
            auto id = cmd.Interpolate(t0 * prarea, t1 * prarea, t2 * prarea);
            cell.buff[y*ScreenGrid::CELL_WIDTH + x] = cmd.Shade(id);
          }
        }
      }
    }
  }
};