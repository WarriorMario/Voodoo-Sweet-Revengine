#include "VArray.h"
#include "ScreenGrid.h"
#include "Graphics.h"
#include "Threading\Job_Manager.h"
#include "Utility\Profiling.h"
#include "SIMD\SIMD.h"

class Rasterizer;

template<typename Shader>
struct alignas(128) RasterizeCellData
{
  RasterizeCellData()
  {}
  RasterizeCellData(Rasterizer& raster, ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
    :
    rasterizer(&raster),
    grid(&grid),
    cell(&cell),
    commands(&commands),
    cell_x(cell_x),
    cell_y(cell_y)
  {

  }
  Rasterizer* rasterizer;
  ScreenGrid* grid;
  ScreenGridCell* cell;
  Array<Shader>* commands;
  int cell_x;
  int cell_y;
};


class Rasterizer
{
public:
  // rasterize cells
  //void RasterizeCells(ScreenGridCell* cells,
  //  Triangle* tri_buff, Shader shader,
  //  int cell_width, int cell_height,
  //  int grid_width, int grid_height);
  template<typename Shader>
  void RasterizeCells(ScreenGrid& grid, Array<Shader>& commands, bool debug = false)
  {
    PROFILE_SCOPE("Render::RenderPass::RasterizeCells");
    if(debug == true)
    {
      for(int y = 0; y < ScreenGrid::HEIGHT; ++y)
      {
        for(int x = 0; x < ScreenGrid::WIDTH; ++x)
        {
          ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
          if(cell.num_indices == 0)
          {
            continue;
          }
          RasterizeCellDebugNumTriangles(grid, cell, commands,
            x, y);
        }
      }
    }
    else
    {

#if 1
      static RasterizeCellData<Shader> job_data[ScreenGrid::NUM_CELLS];
      int job_id = 0;
      if(Shader::SIMD == true)
      {
        for(int y = 0; y < ScreenGrid::HEIGHT; y += 1)
        {
          for(int x = 0; x < ScreenGrid::WIDTH; x += 1)
          {
            ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
            if(cell.num_indices == 0)// Check if this really is a perf gain in release after threading
            {
              continue;
            }
            job_data[job_id] = RasterizeCellData<Shader>(*this, grid, cell, commands, x, y);
            Job s = {RasterizeThreadedCellSIMD<Shader> ,(void*)&(job_data[job_id])};
            JM_SubmitJob(JM_Get(), &s);
            ++job_id;
          }
        }
      }
      else if(Shader::CHECKER == true)
      {
        for(int y = 0; y < ScreenGrid::HEIGHT; y += 1)
        {
          for(int x = 0; x < ScreenGrid::WIDTH; x += 1)
          {
            ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
            if(cell.num_indices == 0)// Check if this really is a perf gain in release after threading
            {
              continue;
            }
            job_data[job_id] = RasterizeCellData<Shader>(*this, grid, cell, commands, x, y);
            Job s = {RasterizeThreadedCellChecker<Shader> ,(void*)&(job_data[job_id])};
            JM_SubmitJob(JM_Get(), &s);
            ++job_id;
          }
        }
      }
      else
      {
        for(int y = 0; y < ScreenGrid::HEIGHT; y += 1)
        {
          for(int x = 0; x < ScreenGrid::WIDTH; x += 1)
          {
            ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
            if(cell.num_indices == 0)// Check if this really is a perf gain in release after threading
            {
              continue;
            }
            job_data[job_id] = RasterizeCellData<Shader>(*this, grid, cell, commands, x, y);
            Job s = {RasterizeThreadedCell<Shader> ,(void*)&(job_data[job_id])};
            JM_SubmitJob(JM_Get(), &s);
            ++job_id;
          }
        }
      }
      //for(int y = 0; y < ScreenGrid::HEIGHT; ++y)
      //{
      //  for(int x = 1; x < ScreenGrid::WIDTH; x+=2)
      //  {
      //    ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
      //    if(cell.num_indices == 0)// Check if this really is a perf gain in release after threading
      //    {
      //      continue;
      //    }
      //    job_data[job_id] = RasterizeCellData<Shader>(*this, grid, cell, commands, x, y);
      //    Job s = {RasterizeThreadedCell<Shader> ,(void*)&(job_data[job_id])};
      //    JM_SubmitJob(JM_Get(), &s);
      //    ++job_id;
      //  }
      //}
      JM_Sync(JM_Get());
#else    
      for(int y = 0; y < ScreenGrid::HEIGHT; y += 1)
      {
        for(int x = 0; x < ScreenGrid::WIDTH; x += 1)
        {
          ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
          if(cell.num_indices == 0)
          {
            continue;
          }
          RasterizeCell(grid, cell, commands,
            x, y);
        }
      }
      //for (int y = 0; y < ScreenGrid::HEIGHT; ++y)
      //{
      //  for (int x = 1; x < ScreenGrid::WIDTH; x+=2)
      //  {
      //	  ScreenGridCell& cell = grid.cells[y * ScreenGrid::WIDTH + x];
      //	  if (cell.num_indices == 0)
      //	  {
      //		  continue;
      //	  }
      //	  RasterizeCell(grid, cell, commands,
      //		  x, y);
      //  }
      //}
#endif
    }
  }
  Color block_colors[3] {Colors::Blue,Colors::Red,Colors::Green};
  template<typename Shader>
  void RasterizeCell(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    SINGLE_CORE_PROFILE("RasterizeCell");
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;
    for(int i = 0; i < cell.num_indices; ++i)
    {
      auto& cmd = commands[cell.indices[i]];
      auto d = cmd.GetPrimData();
      const Vec2 A(d[1].x - d[0].x, d[1].y - d[0].y);
      const Vec2 B(d[2].x - d[0].x, d[2].y - d[0].y);
      const float inv_area = 1.f / (A.x * B.y - A.y * B.x);

      // precalculate barycentric coordinate data
      const int
        a01 = d[0].y - d[1].y, b01 = d[1].x - d[0].x,
        a12 = d[1].y - d[2].y, b12 = d[2].x - d[1].x,
        a20 = d[2].y - d[0].y, b20 = d[0].x - d[2].x;

      int pix_x = start_x;
      int pix_y = start_y;

      int t0_row = (start_x - d[1].x) * a12 - (start_y - d[1].y) * (d[1].x - d[2].x);
      int t1_row = (start_x - d[2].x) * a20 - (start_y - d[2].y) * (d[2].x - d[0].x);
      int t2_row = (start_x - d[0].x) * a01 - (start_y - d[0].y) * (d[0].x - d[1].x);

      Color* cur_color_even = cell.buff;
      Color* cur_color_uneven = cell.checker_buff;
      SINGLE_CORE_PROFILE("RasterizeCellSimd y loop");
      //if(do_even_pixels == true)
      {
        bool offset = true;
        for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y, ++pix_y)
        {
          offset = !offset;
          int t0 = t0_row+ (offset? 0: a12);
          int t1 = t1_row + (offset ? 0 : a20);
          int t2 = t2_row + (offset ? 0 : a01);
          pix_x = start_x + offset;

          SINGLE_CORE_PROFILE("RasterizeCellSimd x loop");
          for(int x = 0; x < ScreenGrid::CELL_WIDTH/2; ++x, pix_x+=2, ++cur_color_even)
          {
            const bool is_inside = (t0 | t1 | t2) >= 0;
            if(is_inside == true)
            {
              // Determine interpolated values
              const InterpData interp = {t1 * inv_area, t2 * inv_area, pix_x, pix_y};
              cmd.Shade(interp, *cur_color_even);
              //*cur_color = is_inside * -1;
            }

            // two steps to the right
            t0 += a12+ a12;
            t1 += a20 + a20;
            t2 += a01 + a01;
          }
          // one step down
          t0_row += b12;
          t1_row += b20;
          t2_row += b01;
        }
      }
      //else

      // precalculate barycentric coordinate data
      pix_x = start_x;
      pix_y = start_y;

      t0_row = (start_x - d[1].x) * a12 - (start_y - d[1].y) * (d[1].x - d[2].x);
      t1_row = (start_x - d[2].x) * a20 - (start_y - d[2].y) * (d[2].x - d[0].x);
      t2_row = (start_x - d[0].x) * a01 - (start_y - d[0].y) * (d[0].x - d[1].x);
      {
        bool offset = false;
        for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y, ++pix_y)
        {
          offset = !offset;
          int t0 = t0_row + (offset ? 0 : a12);
          int t1 = t1_row + (offset ? 0 : a20);
          int t2 = t2_row + (offset ? 0 : a01);
          pix_x = start_x + offset;

          SINGLE_CORE_PROFILE("RasterizeCellSimd x loop");
          for(int x = 0; x < ScreenGrid::CELL_WIDTH/2; ++x, pix_x+=2, ++cur_color_uneven)
          {
            const bool is_inside = (t0 | t1 | t2) >= 0;
            if(is_inside == true)
            {
              // Determine interpolated values
              const InterpData interp = {t1 * inv_area, t2 * inv_area, pix_x, pix_y};
              cmd.Shade(interp, *cur_color_uneven);
              //*cur_color = is_inside * -1;
            }

            // one step to the right
            t0 += a12 + a12;
            t1 += a20 + a20;
            t2 += a01 + a01;
          }
          // one step down
          t0_row += b12;
          t1_row += b20;
          t2_row += b01;
        }
      }
    }
    cell.num_indices = 0;
  }

  template<typename Shader>
  void RasterizeCellDebugNumTriangles(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;

    for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
    {
      for(int x = 0; x < ScreenGrid::CELL_WIDTH/2; ++x)
      {
        for(int j = 0; j < cell.num_indices; ++j)
        {
          float redness = (float)cell.num_indices / CountOf(cell.indices);
          cell.buff[y*ScreenGrid::CELL_WIDTH / 2 + x] = Color((redness)*255.0f, (1.0f - redness)*255.0f, 0);
          cell.checker_buff[y*ScreenGrid::CELL_WIDTH / 2 + x] = Color((redness)*255.0f, (1.0f - redness)*255.0f, 0);
        }
      }
    }
    cell.num_indices = 0;
  }

  template<typename Shader>
  void RasterizeCellSIMD(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    SINGLE_CORE_PROFILE("RasterizeCellSimd primitve");
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;

    for(int i = 0; i < cell.num_indices; ++i)
    {
      auto& cmd = commands[cell.indices[i]];
      auto d = cmd.GetPrimData();
      const Vec2 A(d[1].x - d[0].x, d[1].y - d[0].y);
      const Vec2 B(d[2].x - d[0].x, d[2].y - d[0].y);
      const float area = (A.x * B.y - A.y * B.x);// Fits in 1<<21
      int test = area;
      const float inv_area = 1.f / area;// split up in 1/width and 1/height as that does not require a lot of bits.
      __m256 inv_area8 = AVX_FLOAT_FROM1(inv_area);

      // precalculate barycentric coordinate data
      int// A -1080 to 1080, B -1920 t0 1920 both fits in 1<<11 + 1 for sign
        a01 = d[0].y - d[1].y, b01 = d[1].x - d[0].x,
        a12 = d[1].y - d[2].y, b12 = d[2].x - d[1].x,
        a20 = d[2].y - d[0].y, b20 = d[0].x - d[2].x;
      
      __m256i a018s = AVX_INT32_FROM1(a01);
      __m256i a128s = AVX_INT32_FROM1(a12);
      __m256i a208s = AVX_INT32_FROM1(a20);
      __m256i a018 = AVX_INT32_FROM1(a01 << 4);
      __m256i a128 = AVX_INT32_FROM1(a12 << 4);
      __m256i a208 = AVX_INT32_FROM1(a20 << 4);

      //__m256i a018_starting_step = AVX_INT32_FROM8(0, a12, a12 << 1, (a12 << 1) + a12, a12 << 2, (a12 << 2) + a12, (a12 << 2) + (a12 << 1), (a12 << 2) + (a12 << 1) + a12);
      //__m256i a128_starting_step = AVX_INT32_FROM8(0, a20, a20 << 1, (a20 << 1) + a20, a20 << 2, (a20 << 2) + a20, (a20 << 2) + (a20 << 1), (a20 << 2) + (a20 << 1) + a20);
      //__m256i a208_starting_step = AVX_INT32_FROM8(0, a01, a01 << 1, (a01 << 1) + a01, a01 << 2, (a01 << 2) + a01, (a01 << 2) + (a01 << 1), (a01 << 2) + (a01 << 1) + a01);
      __m256i a018_starting_step = AVX_INT32_FROM8(0, a12 << 1, a12 << 2, (a12 << 2) + (a12 << 1), a12 << 3, (a12 << 3) + (a12 << 1), (a12 << 3) + (a12 << 2), (a12 << 3) + (a12 << 2) + (a12 << 1));
      __m256i a128_starting_step = AVX_INT32_FROM8(0, a20 << 1, a20 << 2, (a20 << 2) + (a20 << 1), a20 << 3, (a20 << 3) + (a20 << 1), (a20 << 3) + (a20 << 2), (a20 << 3) + (a20 << 2) + (a20 << 1));
      __m256i a208_starting_step = AVX_INT32_FROM8(0, a01 << 1, a01 << 2, (a01 << 2) + (a01 << 1), a01 << 3, (a01 << 3) + (a01 << 1), (a01 << 3) + (a01 << 2), (a01 << 3) + (a01 << 2) + (a01 << 1));

      int pix_x = start_x;
      int pix_y = start_y;

      int t0_row = (start_x - d[1].x) * a12 - (start_y - d[1].y) * (d[1].x - d[2].x);// Fits in 1<<21
      int t1_row = (start_x - d[2].x) * a20 - (start_y - d[2].y) * (d[2].x - d[0].x);// Fits in 1<<21
      int t2_row = (start_x - d[0].x) * a01 - (start_y - d[0].y) * (d[0].x - d[1].x);// Fits in 1<<21

      __m256i t08_row = AVX_INT32_ADD(AVX_INT32_FROM1(t0_row), a018_starting_step);
      __m256i t18_row = AVX_INT32_ADD(AVX_INT32_FROM1(t1_row), a128_starting_step);
      __m256i t28_row = AVX_INT32_ADD(AVX_INT32_FROM1(t2_row), a208_starting_step);
      Color* cur_color = cell.buff;
      if(do_even_pixels == true)
      {
        cur_color = cell.checker_buff;
      }
      bool offset = do_even_pixels;
      SINGLE_CORE_PROFILE("RasterizeCellSimd y loop");
      for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y, ++pix_y)
      {
        offset = !offset;
        //int t0 = t0_row + (offset ? 0 : a12);
        //int t1 = t1_row + (offset ? 0 : a20);
        //int t2 = t2_row + (offset ? 0 : a01);
        __m256i t08 = t08_row;
        __m256i t18 = t18_row;
        __m256i t28 = t28_row;
        if(offset == true)
        {
          t08 = AVX_INT32_ADD(t08, a128s);
          t18 = AVX_INT32_ADD(t18, a208s);
          t28 = AVX_INT32_ADD(t28, a018s);
        }
        pix_x = start_x + offset;

        SINGLE_CORE_PROFILE("RasterizeCellSimd x loop");
        for(int x = 0; x < ScreenGrid::CELL_WIDTH / 16; ++x, pix_x += 16, cur_color += 8)
        {
          //const bool is_inside = (t0 | t1 | t2) >= 0;
          __m256i is_inside8 = _mm256_cmpgt_epi32(_mm256_or_si256(_mm256_or_si256(t08, t18), t28), AVX_INT32_FROM1(-1));

          //if(is_inside == true)
          {
            // Determine interpolated values
            //const InterpData interp = {t1 * inv_area, t2 * inv_area, pix_x, pix_y};
            //SINGLE_CORE_PROFILE("RasterizeCellSimd shade");
            cmd.ShadeSIMD(AVX_FLOAT_MUL(_mm256_cvtepi32_ps(t18), inv_area8), AVX_FLOAT_MUL(_mm256_cvtepi32_ps(t28), inv_area8), is_inside8, pix_x, pix_y, cur_color);

          }

          // two steps to the right
          t08 = AVX_INT32_ADD(t08, a128);
          t18 = AVX_INT32_ADD(t18, a208);
          t28 = AVX_INT32_ADD(t28, a018);
          //t0 += a12;
          //t1 += a20;
          //t2 += a01;
        }

        // one step down
        t08_row = AVX_INT32_ADD(t08_row, AVX_INT32_FROM1(b12));
        t18_row = AVX_INT32_ADD(t18_row, AVX_INT32_FROM1(b20));
        t28_row = AVX_INT32_ADD(t28_row, AVX_INT32_FROM1(b01));
        //t0_row += b12;
        //t1_row += b20;
        //t2_row += b01;
      }
    }
    cell.num_indices = 0;
  }

  template<typename Shader>
  void RasterizeCellChecker(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    SINGLE_CORE_PROFILE("RasterizeCell");
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;
    for(int i = 0; i < cell.num_indices; ++i)
    {
      auto& cmd = commands[cell.indices[i]];
      auto d = cmd.GetPrimData();
      const Vec2 A(d[1].x - d[0].x, d[1].y - d[0].y);
      const Vec2 B(d[2].x - d[0].x, d[2].y - d[0].y);
      const float inv_area = 1.f / (A.x * B.y - A.y * B.x);

      // precalculate barycentric coordinate data
      int
        a01 = d[0].y - d[1].y, b01 = d[1].x - d[0].x,
        a12 = d[1].y - d[2].y, b12 = d[2].x - d[1].x,
        a20 = d[2].y - d[0].y, b20 = d[0].x - d[2].x;

      int pix_x = start_x;
      int pix_y = start_y;

      int t0_row = (start_x - d[1].x) * a12 - (start_y - d[1].y) * (d[1].x - d[2].x);
      int t1_row = (start_x - d[2].x) * a20 - (start_y - d[2].y) * (d[2].x - d[0].x);
      int t2_row = (start_x - d[0].x) * a01 - (start_y - d[0].y) * (d[0].x - d[1].x);

      Color* cur_color = cell.buff;
      if(do_even_pixels == false)
      {
        cur_color = cell.checker_buff;
      }
      SINGLE_CORE_PROFILE("RasterizeCellSimd y loop");
      //if(do_even_pixels == true)
      {
        bool offset = do_even_pixels;
        for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y, ++pix_y)
        {
          offset = !offset;
          int t0 = t0_row + (offset ? 0 : a12);
          int t1 = t1_row + (offset ? 0 : a20);
          int t2 = t2_row + (offset ? 0 : a01);
          pix_x = start_x + offset;

          SINGLE_CORE_PROFILE("RasterizeCellSimd x loop");
          for(int x = 0; x < ScreenGrid::CELL_WIDTH / 2; ++x, pix_x += 2, ++cur_color)
          {
            const bool is_inside = (t0 | t1 | t2) >= 0;
            if(is_inside == true)
            {
              // Determine interpolated values
              const InterpData interp = {t1 * inv_area, t2 * inv_area, pix_x, pix_y};
              cmd.Shade(interp, *cur_color);
            }

            // two steps to the right
            t0 += a12 + a12;
            t1 += a20 + a20;
            t2 += a01 + a01;
          }
          // one step down
          t0_row += b12;
          t1_row += b20;
          t2_row += b01;
        }
      }
    }
    cell.num_indices = 0;
  }

  void FlipCheckerBoard()
  {
    do_even_pixels = !do_even_pixels;
  }
  private:
  bool do_even_pixels = false;
};


template<typename Shader>
void RasterizeThreadedCell(void* data)
{
  RasterizeCellData<Shader>* real_data = (RasterizeCellData<Shader>*)data;
  real_data->rasterizer->RasterizeCell(*real_data->grid, *real_data->cell, *real_data->commands, real_data->cell_x, real_data->cell_y);
}
template<typename Shader>
void RasterizeThreadedCellChecker(void* data)
{
  RasterizeCellData<Shader>* real_data = (RasterizeCellData<Shader>*)data;
  real_data->rasterizer->RasterizeCellChecker(*real_data->grid, *real_data->cell, *real_data->commands, real_data->cell_x, real_data->cell_y);
}
template<typename Shader>
void RasterizeThreadedCellSIMD(void* data)
{
  RasterizeCellData<Shader>* real_data = (RasterizeCellData<Shader>*)data;
  real_data->rasterizer->RasterizeCellSIMD(*real_data->grid, *real_data->cell, *real_data->commands, real_data->cell_x, real_data->cell_y);
}