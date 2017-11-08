#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Colors.h"
#include "Utility.h"


struct ScreenGridCell
{
  size_t num_indices;
  size_t indices[64];
  Color* buff;
};
template<int width, int height, int cell_width, int cell_height>
class ScreenGrid
{
  static constexpr size_t RESOLUTION_X = width * cell_width;
  static constexpr size_t RESOLUTION_Y = height * cell_height;

public:
  static constexpr size_t NUM_CELLS = width*height;
  static constexpr size_t WIDTH = width;
  static constexpr size_t HEIGHT= height;
  ScreenGrid()
  {
    static_assert(IsPowerOf2(cell_width));
    static_assert(IsPowerOf2(cell_height));

    buff = new Color[RESOLUTION_X * RESOLUTION_Y];
    int cell_buff_sz = cell_width * cell_height;
    int offset = 0;
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j, offset += cell_buff_sz)
      {
        Cell& cell = cells[i * height + j];
        cell.num_indices = 0;
        cell.buff = buff + offset;
      }
    }
  }
  ~ScreenGrid()
  {
    delete[] buff;
  }

  void PlaceTriangleInCell(Vec3 points[3], int primitive_index)
  {}

  void PlaceAABBInCell(RectI rect, int primitive_index)
  {
    // Calculate grid start and end points
    size_t start_x = Max(rect.left / cell_width, 0);
    size_t start_y = rect.top / cell_width;
    size_t end_x = rect.right / cell_width;
    size_t end_y = rect.bottom / cell_width;

    for(int y = start_y; y < end_y; ++y)
    {
      for(int x = start_x; x < end_x; ++x)
      {
        Cell& cell = cells[y*height + x];
        cell.indices[cell.num_indices] = primitive_index;
        cell.num_indices++;
      }
    }
  }

  void UnPackBuffer(Color* destination)
  {

  }

  Cell cells[width * height];
  Color* buff;
};