#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Colors.h"
#include "Utility.h"

// ****************************************************************************
struct ScreenGridCell
{
  size_t num_indices;
  size_t indices[64];
  Color* buff;
};
template<int width, int height, int cell_width, int cell_height>
class ScreenGridImpl
{
  static constexpr size_t RESOLUTION_X = width * cell_width;
  static constexpr size_t RESOLUTION_Y = height * cell_height;

public:
  static constexpr size_t NUM_CELLS = width*height;
  static constexpr size_t NUM_CELL_PIXELS = cell_width * cell_height;
  static constexpr size_t WIDTH = width;
  static constexpr size_t HEIGHT= height;
  ScreenGridImpl()
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
        ScreenGridCell& cell = cells[i * height + j];
        cell.num_indices = 0;
        cell.buff = buff + offset;
      }
    }
  }
  ~ScreenGridImpl()
  {
    delete[] buff;
  }

  void PlaceTriangleInCell(Vec3 points[3], int primitive_index)
  {

  }

  void PlaceAABBInCell(RectI rect, int primitive_index, int primitve_index_2)
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
        cell.indices[cell.num_indices] = primitve_index_2;
        cell.num_indices++;
      }
    }
  }

  void UnPackBuffer(Color* destination)
  {
    for(int i = 0; i < NUM_CELLS; ++i)
    {
      memmove(destination, cells[i].buff, NUM_CELL_PIXELS * sizeof(Color));
      destination += NUM_CELL_PIXELS;
    }
  }

  ScreenGridCell cells[width * height];
  Color* buff;
};

// ****************************************************************************
using ScreenGrid = ScreenGridImpl<60, 135, 32, 8>;
//using ScreenGrid = ScreenGridImpl<60, 34, 32, 32>;
//using ScreenGrid = ScreenGridImpl<30, 17, 64, 64>
//using ScreenGrid = ScreenGridImpl<15, 17, 128, 64>;