#include "Rasterizer.h"
#include "ScreenGrid.h"
#include "Vec2.h"

void Rasterizer::RasterizeCells(ScreenGridCell* cells,
  Triangle* tri_buff, Shader shader,
  int cell_width, int cell_height,
  int grid_width, int grid_height)
{
  for(int y = 0; y < grid_height; ++y)
  {
    for(int x = 0; x < grid_width; ++x)
    {
      RasterizeCell(cells[y * grid_width + x], tri_buff, shader,
        x, y, cell_width, cell_height);
    }
  }
}

void Rasterizer::RasterizeCell(ScreenGridCell & cell,
  Triangle* tri_buff, Shader shader,
  int cell_x, int cell_y,
  int cell_width, int cell_height)
{
  int start_x = cell_x * cell_width, start_y = cell_y * cell_height;
  Color randColor;
  randColor.SetR(std::rand() % 255);
  randColor.SetG(std::rand() % 255);
  randColor.SetB(std::rand() % 255);
  randColor.SetA(255);
  for(int y = 0; y < cell_height; ++y)
  {
    for(int x = 0; x < cell_width; ++x)
    {
      if(((y + start_y) > (Graphics::ScreenHeight - 1)) ||
        ((x + start_x) >(Graphics::ScreenWidth - 1)))
      {
        continue;
      }

      shader.gfx.PutPixel((x + start_x), (y + start_y), randColor);
      for(int j = 0; j < cell.num_indices; ++j)
      {
        Vec2 p = Vec2((float)(x + start_x), (float)(y + start_y));
        Triangle& t = tri_buff[cell.indices[j]];
        
        bool is_inside = true;
        is_inside &= IsRightOfLine(t.p[0], t.p[1], p);
        is_inside &= IsRightOfLine(t.p[1], t.p[2], p);
        is_inside &= IsRightOfLine(t.p[2], t.p[0], p);

        if(is_inside == true)
        {
          if(j == 0)
          {
            //shader.Shade((x + start_x), (y + start_y));
          }
          else
          {
            shader.Shade((x + start_x), (y + start_y));
          }
        }
      }
    }
  }
}

bool Rasterizer::IsRightOfLine(const Vec2& v1, const Vec2& v2, const Vec2& p)
{
  return ((p.x - v1.x) * (v2.y - v1.y) - (p.y - v1.y) * (v2.x - v1.x) <= 0);
}
