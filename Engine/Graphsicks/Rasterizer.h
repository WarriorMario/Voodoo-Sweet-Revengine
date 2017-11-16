#include "VArray.h"
#include "ScreenGrid.h"
#include "Graphics.h"
#include "Threading\Job_Manager.h"
#include "Utility\Profiling.h"
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

  template<typename Shader>
  void RasterizeCell(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;

#if 1
    for(int i = 0; i < cell.num_indices; ++i)
    {
      auto& cmd = commands[cell.indices[i]];
      auto* d = cmd.prim_data;
      const Vec2 A(d[1].x - d[0].x, d[1].y - d[0].y);
      const Vec2 B(d[2].x - d[0].x, d[2].y - d[0].y);
      const float prarea = 1.f / (A.x * B.y - A.y * B.x);

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

      for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y, ++pix_y)
      {
        int t0 = t0_row;
        int t1 = t1_row;
        int t2 = t2_row;
        pix_x = start_x;

        for(int x = 0; x < ScreenGrid::CELL_WIDTH; ++x, ++pix_x)
        {
          const bool is_inside = (t0 | t1 | t2) >= 0;
          if(is_inside == true)
          {
            // Determine interpolated values
            const InterpData interp = {t0 * prarea, t1 * prarea, t2 * prarea, pix_x, pix_y};
            Shader::PixelData id;
            cmd.Interpolate(interp, id);
            cmd.Shade(id, cell.buff[y*ScreenGrid::CELL_WIDTH + x]);
          }

          // one step to the right
          t0 += a12;
          t1 += a20;
          t2 += a01;
        }

        // one step down
        t0_row += b12;
        t1_row += b20;
        t2_row += b01;
      }
    }
#else
    for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
    {
      for(int x = 0; x < ScreenGrid::CELL_WIDTH; ++x)
      {
        for(int i = 0; i < cell.num_indices; ++i)
        {
          auto& cmd = commands[cell.indices[i]];
          auto* d = cmd.prim_data;
          Vec2 p = Vec2((float)(x + start_x), (float)(y + start_y));

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
            InterpData interp = {t0 * prarea, t1 * prarea, t2 * prarea, (int)p.x, (int)p.y};
            auto id = cmd.Interpolate(interp);
            cell.buff[y*ScreenGrid::CELL_WIDTH + x] = cmd.Shade(id);
          }
        }
      }
    }
#endif
    cell.num_indices = 0;
  }

  template<typename Shader>
  void RasterizeCellDebugNumTriangles(ScreenGrid& grid, ScreenGridCell& cell, Array<Shader>& commands, int cell_x, int cell_y)
  {
    int start_x = cell_x * ScreenGrid::CELL_WIDTH, start_y = cell_y * ScreenGrid::CELL_HEIGHT;

    for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
    {
      for(int x = 0; x < ScreenGrid::CELL_WIDTH; ++x)
      {
        for(int j = 0; j < cell.num_indices; ++j)
        {
          float redness = (float)cell.num_indices / (sizeof(cell.indices) / sizeof(int));
          cell.buff[y*ScreenGrid::CELL_WIDTH + x] = Color((redness)*255.0f, (1.0f - redness)*255.0f, 0);
        }
      }
    }
    cell.num_indices = 0;
  }

};


template<typename Shader>
void RasterizeThreadedCell(void* data)
{
  RasterizeCellData<Shader>* real_data = (RasterizeCellData<Shader>*)data;
  real_data->rasterizer->RasterizeCell(*real_data->grid, *real_data->cell, *real_data->commands, real_data->cell_x, real_data->cell_y);
}