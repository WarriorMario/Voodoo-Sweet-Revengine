#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Colors.h"
#include "Utility.h"

// ****************************************************************************
struct ScreenGridCell
{
  size_t num_indices;
  size_t indices[8];
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
  static constexpr size_t CELL_WIDTH = cell_width;
  static constexpr size_t CELL_HEIGHT = cell_height;

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
        ScreenGridCell& cell = cells[i * width + j];
        cell.num_indices = 0;
        cell.buff = buff + offset;
      }
    }
  }
  ~ScreenGridImpl()
  {
    delete[] buff;
  }

  void PlaceTriangleInCell(Vec2 points[3], int primitive_index)
  {
	  int maxX = Max(Max(points[0].x, points[1].x), points[2].x);
	  int maxY = Min(Min(points[0].y, points[1].y), points[2].y);
	  int minX = Min(Min(points[0].x, points[1].x), points[2].x);
	  int minY = Max(Max(points[0].y, points[1].y), points[2].y);
	  PlaceAABBInCell(RectI(minY, maxY, minX, maxX), primitive_index, primitive_index);
  }

  void PlaceAABBInCell(RectI rect, int primitive_index, int primitve_index_2)
  {
    // Calculate grid start and end points
    size_t start_x = Max(rect.left / cell_width, 0);
    size_t start_y = rect.bottom / cell_height;
    size_t end_x = (rect.right + cell_width - 1) / cell_width;
    size_t end_y = (rect.top + cell_height - 1) / cell_height;

    for(int y = start_y; y < end_y; ++y)
    {
      for(int x = start_x; x < end_x; ++x)
      {
        ScreenGridCell& cell = cells[y * width + x];
        cell.indices[cell.num_indices] = primitive_index;
        cell.num_indices++;
        cell.indices[cell.num_indices] = primitve_index_2;
        cell.num_indices++;
      }
    }
  }

  void UnPackBuffer(Color* destination)
  {
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j)
      {
        ScreenGridCell& cell = cells[i * width + j];
        Color* cur = &destination[(i * CELL_HEIGHT)*Graphics::ScreenWidth + j*CELL_WIDTH];
        for(int y = 0; y < cell_height; ++y)
        {
          memcpy(cur, &cell.buff[y*cell_width], sizeof(Color)*ScreenGrid::CELL_WIDTH);
          cur += Graphics::ScreenWidth;
        }
      }
    }
  }

  void Clear()
  {
    for(int i = 0; i < NUM_CELLS ; ++i)
    {
      cells[i].num_indices = 0;
    }
  }

  ScreenGridCell cells[width * height];
  Color* buff;
};

// ****************************************************************************
//using ScreenGrid = ScreenGridImpl<60, 135, 32, 8>;
using ScreenGrid = ScreenGridImpl<25, 75, 32, 8>;
//using ScreenGrid = ScreenGridImpl<30, 17, 64, 64>
//using ScreenGrid = ScreenGridImpl<15, 17, 128, 64>;