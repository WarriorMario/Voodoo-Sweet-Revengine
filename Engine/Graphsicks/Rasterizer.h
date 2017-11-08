#include "VArray.h"
#include "ScreenGrid.h"

class Rasterizer
{
public:
  // rasterize cells
  //void RasterizeCells(ScreenGridCell* cells,
  //  Triangle* tri_buff, Shader shader,
  //  int cell_width, int cell_height,
  //  int grid_width, int grid_height);

private:
  // rasterize a single cell. Utility function for public RasterizeCells
  //void RasterizeCell(ScreenGridCell& cell,
  //  Triangle* tri_buff, Shader shader,
  //  int cell_x, int cell_y,
  //  int cell_width, int cell_height);

  // see if the point is to the right of the line. Utility function for RasterizeCell
  bool IsRightOfLine(const Vec2& v1, const Vec2& v2, const Vec2& p);
  template<typename Shader>
  void RasterizeCells(ScreenGrid& grid, Array<Shader>& commands)
  {
	  for(int y = 0; y < ScreenGrid::HEIGHT; ++y)
	  {
		  for(int x = 0; x < ScreenGrid::WIDTH; ++x)
		  {
			  RasterizeCell(ScreenGrid,cells[y * ScreenGrid::WIDTH + x],
				  x, y);
		  }
	  }
  }

  template<typename Shader>
  void RasterizeCell(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
	  int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;
	  Color randColor;
	  randColor.SetR(std::rand() % 255);
	  randColor.SetG(std::rand() % 255);
	  randColor.SetB(std::rand() % 255);
	  randColor.SetA(255);
	  for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
	  {
		  for(int x = 0; x <  ScreenGrid::CELL_WIDTH; ++x)
		  {
			  if(((y + start_y) >(Graphics::ScreenHeight - 1)) ||
				  ((x + start_x) >(Graphics::ScreenWidth - 1)))
			  {
				  continue;
			  }

			  shader.gfx.PutPixel((x + start_x), (y + start_y), randColor);
			  for(int j = 0; j < cell.num_indices; ++j)
			  {
				  Vec2 p = Vec2((float)(x + start_x), (float)(y + start_y));
				  Vec2* t = commands[cell.indices[j]].prim_data;

				  bool is_inside = true;
				  is_inside &= IsRightOfLine(t[0], t[1], p);
				  is_inside &= IsRightOfLine(t[1], t[2], p);
				  is_inside &= IsRightOfLine(t[2], t[0], p);

				  if(is_inside == true)
				  {
					  if(j == 0)
					  {
						  //shader.Shade((x + start_x), (y + start_y));
					  }
					  else
					  {
						  //shader.Shade((x + start_x), (y + start_y));
					  }
				  }
			  }
		  }
	  }
  }
};