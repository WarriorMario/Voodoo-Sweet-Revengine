#pragma once
#include "Matrix.h"
#include "Rect.h"
#include "Colors.h"
#include "Utility.h"
#include "Utility\Profiling.h"
// ****************************************************************************
struct ScreenGridCell
{
  size_t num_indices;
  unsigned short indices[64];
  Color* buff;
  Color* checker_buff;
};

struct alignas(128) UnPackBufferData
{
  Color* destination;
  ScreenGridCell* cell_line_start;
  size_t y;
};


void ThreadedUnPackBuffer(void* data);

template<int width, int height, int cell_width, int cell_height>
class ScreenGridImpl
{
  static constexpr size_t RESOLUTION_X = width * cell_width;
  static constexpr size_t RESOLUTION_Y = height * cell_height;

public:
  static constexpr size_t NUM_CELLS = width * height;
  static constexpr size_t NUM_CELL_PIXELS = cell_width * cell_height;
  static constexpr size_t NUM_PIXELS = RESOLUTION_X * RESOLUTION_Y;
  static constexpr size_t WIDTH = width;
  static constexpr size_t HEIGHT = height;
  static constexpr size_t CELL_WIDTH = cell_width;
  static constexpr size_t CELL_HEIGHT = cell_height;

  ScreenGridImpl()
  {
    //static_assert(IsPowerOf2(cell_width));
    //static_assert(IsPowerOf2(cell_height));
    buff = (Color*)_aligned_malloc(RESOLUTION_X * RESOLUTION_Y * sizeof Color, 128);
    memset(buff, 0, RESOLUTION_X * RESOLUTION_Y * sizeof Color);
    int cell_buff_sz = (cell_width / 2) * cell_height;
    int offset = 0;
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j, offset += cell_buff_sz)
      {
        ScreenGridCell& cell = cells[i * width + j];
        cell.num_indices = 0;
        cell.buff = buff + offset;
        cell.checker_buff = buff + offset + NUM_PIXELS / 2;
      }
    }
  }
  ~ScreenGridImpl()
  {
    _aligned_free(buff);
  }
  template<typename T>
  class SmartArray
  {
  public:
    //...
    int size;
    T* array;
    T &operator[](size_t index)
    {
      if(index / size != 0)
        throw  0x0A;
      return array[index];
    }
  };

  void PlaceTriangleInCell(Vec2 points[3], int primitive_index)
  {
    PROFILE_SCOPE("Render::Grid::PlaceTriangleInCell");
    int a_start[height];
    SmartArray<int> v_start;
    v_start.size = height;
    v_start.array = a_start;
    for(int i = 0; i < height; ++i)
    {
      v_start[i] = 100000;
    }
    int a_end[height];
    SmartArray<int> v_end;
    v_end.size = height;
    v_end.array = a_end;
    memset(a_end, -1, sizeof(int)*height);

    int y_min = height-1;
    int y_max = -1;
    for(int iEdge = 0; iEdge < 3; ++iEdge)
    {
      Vec2 start = points[iEdge];
      Vec2 end = points[(iEdge + 1) == 3 ? 0 : (iEdge + 1)];
      if(start.y > end.y)
      {
        b2Swap(start, end);
      }
      float y0 = start.y;
      float y1 = end.y;
      float x0 = start.x;
      float x1 = end.x;
      if((end.y - start.y) == 0.0f)
      {
        continue;
      }

      float dx = x1 - x0;
      float dy = y1 - y0;
      float nx = (abs(dx));
      float ny = (abs(dy));
      float sign_x = dx > 0.0f ? 1 : -1;
      float sign_y = dy > 0.0f ? 1 : -1;

      float point_x = x0;
      float point_y = y0;

      y_min = Max(Min((int)(point_y / cell_height), y_min), 0);

      v_start[y_min] = Min((int)(point_x / cell_width), v_start[y_min]);
      v_end[y_min] = Max((int)(point_x / cell_width), v_end[y_min]);

      for(int ix = 0, iy = 0; ix < nx || iy < ny;)
      {
        size_t y_index = (point_y / cell_height);
        int x_index = (point_x / cell_width);
        if((0.5f + ix) / nx == (0.5f + iy) / ny)
        {
          // next step is diagonal
          // Add point
          point_x += sign_x;
          x_index = (point_x / cell_width);
          size_t old_x = x_index;
          if(y_index < height&& x_index >= 0)
          {
            v_start[y_index] = Min(x_index, v_start[y_index]);
            v_end[y_index] = Max(x_index, v_end[y_index]);
          }
          // subtract x add y Add point
          point_y += sign_y;
          y_index = (point_y / cell_height);
          point_x -= sign_x;
          x_index = (point_x / cell_width);
          if(y_index < height&& x_index >= 0)
          {
            v_start[y_index] = Min(x_index, v_start[y_index]);
            v_end[y_index] = Max(x_index, v_end[y_index]);
          }
          // Add x Add point
          point_x += sign_x;
          x_index = old_x;
          if(y_index < height&& x_index >= 0)
          {
            v_start[y_index] = Min(x_index, v_start[y_index]);
            v_end[y_index] = Max(x_index, v_end[y_index]);
          }
          ix++;
          iy++;

        }
        else if((0.5f + ix) / nx < (0.5f + iy) / ny)
        {
          // next step is horizontal
          point_x += sign_x;
          x_index = (point_x / cell_width);
          if(y_index < height&& x_index >= 0)
          {
            v_start[y_index] = Min(x_index, v_start[y_index]);
            v_end[y_index] = Max(x_index, v_end[y_index]);
          }
          // Add point
          ix++;

        }
        else
        {
          // next step is vertical
          point_y += sign_y;
          y_index = (point_y / cell_height);
          //if((point_y/ cell_height) >= 0 && (point_y/ cell_height) < Graphics::ScreenHeight/ cell_height)
          if(y_index < height&& x_index >= 0)
          {
            v_start[y_index] = Min(x_index, v_start[y_index]);
            v_end[y_index] = Max(x_index, v_end[y_index]);
          }
          // Add point
          iy++;

        }
        y_min = Min((int)y_index, y_min);
        y_max = Max((int)y_index, y_max);
      }
    }
    if(y_max < 0)
    {
      return;
    }
    y_max = Min(y_max, height - 1);
    y_min = Max(y_min, 0);

    for(; y_min <= y_max; ++y_min)
    {
      if(v_end[y_min] < 0)
      {
        continue;
      }
      for(int x = Max(v_start[y_min], 0); x <= Min(v_end[y_min], width-1); ++x)
      {
        ScreenGridCell& cell = cells[(y_min)* width + (x)];
        //assert(cell.num_indices < _countof(cell.indices) && "Cell indices out of bounds");
        if(cell.num_indices < _countof(cell.indices))
        {
          cell.indices[cell.num_indices] = primitive_index;
          ++cell.num_indices;
        }
      }
    }
  }

  void PlaceAABBInCell(RectI rect, int primitive_index, int primitve_index_2)
  {
    // Calculate grid start and end points
    size_t start_x = Max(rect.left / cell_width, 0);
    size_t start_y = Max(rect.bottom / cell_height, 0);
    size_t end_x = Min((rect.right + cell_width - 1) / cell_width, width);
    size_t end_y = Min((rect.top + cell_height - 1) / cell_height, height);

    for(int y = start_y; y < end_y; ++y)
    {
      for(int x = start_x; x < end_x; ++x)
      {
        ScreenGridCell& cell = cells[y * width + x];
        assert(cell.num_indices < _countof(cell.indices) && "Cell indices out of bounds");
        cell.indices[cell.num_indices] = primitive_index;
        cell.num_indices++;
        cell.indices[cell.num_indices] = primitve_index_2;
        cell.num_indices++;
      }
    }
  }

  void UnPackBuffer(Color* destination)
  {
    PROFILE_SCOPE("Render::Grid::UnPackBuffer");
#if 0
    UnPackBufferData job_data[height] alignas(128);
    size_t job_id = 0;
    for(int i = 0; i < height; ++i)
    {
      ScreenGridCell& cell = cells[i * width];
      Color* cur = &destination[(i * CELL_HEIGHT)*Graphics::ScreenWidth];
      UnPackBufferData data;
      data.cell_line_start = &cell;
      data.destination = destination;
      data.y = i;
      job_data[job_id] = data;
      Job s = {ThreadedUnPackBuffer ,(void*)&(job_data[job_id])};
      JM_SubmitJob(JM_Get(), &s);
      ++job_id;
}
    JM_Sync(JM_Get());
#else
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j)
      {
        ScreenGridCell& cell = cells[i * width + j];
        Color* cur = &destination[(i * CELL_HEIGHT)*Graphics::ScreenWidth + j * CELL_WIDTH];
        bool even = true;
        for(int y = 0; y < cell_height; ++y)
        {
          int cellheight = cell_height;
          Color* old_cur = cur;
          for(int x = 0; x < cell_width; ++x)
          {
            if(x % 2 == even)
            {
              *cur = cell.buff[y*(cell_width / 2) + (x / 2)];
            }
            else
            {
              *cur = cell.checker_buff[y*(cell_width / 2) + (x / 2)];
            }
            ++cur;
          }
          even = !even;
          cur = old_cur + Graphics::ScreenWidth;
        }
      }
    }
#endif
  }

  void Clear()
  {
    PROFILE_SCOPE("Render::Grid::Clear");
    //for(int i = 0; i < NUM_CELLS; ++i)// Seperate array from cells for more perf if needed
    //{
    //  cells[i].num_indices = 0;
    //}
    //memset(buff, 128, sizeof(Color)*ScreenGrid::RESOLUTION_X * ScreenGrid::RESOLUTION_Y);
  }

  ScreenGridCell cells[width * height];
  Color* buff;
};

// ****************************************************************************
using ScreenGrid = ScreenGridImpl<10, 45, 192, 24>;
//using ScreenGrid = ScreenGridImpl<25, 75, 32, 8>;
//using ScreenGrid = ScreenGridImpl<15, 15, 128, 64>;
//using ScreenGrid = ScreenGridImpl<15, 17, 128, 64>;
