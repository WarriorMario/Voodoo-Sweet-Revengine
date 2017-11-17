#pragma once
#include "Rasterizer.h"
#include "VTuple.h"
#include "VArray.h"
#include "VTraits.h"
#include "Shader.h"

class Graphics;

// ****************************************************************************
template<typename Shader>
class RenderPass
{
public:
  class Pass
  {
  public:
    void Apply(ScreenGrid& grid, Rasterizer& rasterizer)
    {
      for(int i = 0; i < commands.size(); ++i)
      {
        auto pd = commands[i].GetPrimData();
        Vec2 points[3] = {
          {pd[0].x, pd[0].y},
          {pd[1].x, pd[1].y},
          {pd[2].x, pd[2].y}
        };
        grid.PlaceTriangleInCell(points, i);
      }
      if(commands.size() != 0)
      {
        rasterizer.RasterizeCells(grid, commands, false);
        commands.clear();
      }
    }

    Array<Shader> commands;
  } pass;
  class SIMDPass
  {
  public:
    void Apply(ScreenGrid& grid, Rasterizer& rasterizer)
    {
      for(int i = 0; i < prim_data.size; ++i)
      {
        //auto pd = commands[i].GetPrimData();
        Vec2 points[3];
        prim_data.GetTriangle(points, i);
        grid.PlaceTriangleInCell(points, i);
      }
      if(prim_data.size != 0)
      {
        //rasterizer.RasterizeCells(grid, commands, false);
        prim_data.Clear();
      }
    }
    typename Shader::DataSIMDBuffer prim_data;
  } simd_pass;

};

// ****************************************************************************

class Renderer
{
  template<typename... Shaders>
  using Passes = Tuple<RenderPass<Shaders>...>;

  struct PassApplyer
  {
    template<typename T, typename... Args>
    void operator()(T& pass, Args&&... args)
    {
      pass.pass.Apply(Forward<Args>(args)...);
      pass.simd_pass.Apply(Forward<Args>(args)...);
    }
  };

public:
  Renderer(Graphics& gfx);

  template<typename Shader>
  void AddDrawCommand(Shader& s)
  {
    Get<RenderPass<Shader>>(passes).pass.commands.push_back(s);
  }
  template<typename Shader>
  void AddDrawCommandSIMD(Shader& s)
  {
    Get<RenderPass<Shader>>(passes).simd_pass.prim_data.Add(s);
  }
  void Render();

  ScreenGrid grid;
private:
  Rasterizer rasterizer;
  Graphics& gfx;
  Passes<BackgroundShader, ForegroundShader, UIShader, UIShaderSimple> passes;
};

