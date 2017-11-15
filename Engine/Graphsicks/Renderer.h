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
	void Apply(ScreenGrid& grid, Rasterizer& rasterizer)
  {
    for(int i = 0; i < commands.size(); ++i)
    {
      auto& pd = commands[i].prim_data;
      Vec2 points[3] = {
        {pd[0].x, pd[0].y}, 
        {pd[1].x, pd[1].y}, 
        {pd[2].x, pd[2].y}
      };
      grid.PlaceTriangleInCell(points, i);
    }
    if(commands.size() != 0)
    {
      rasterizer.RasterizeCells(grid, commands,false);
      commands.clear();
    }
  }

  Array<Shader> commands;
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
      pass.Apply(Forward<Args>(args)...);
    }
  };

public:
	Renderer(Graphics& gfx);

  template<typename Shader>
  void AddDrawCommand(Shader& s)
  {
    Get<RenderPass<Shader>>(passes).commands.push_back(s);
  }
  void Render();

  ScreenGrid grid;
private:
  Rasterizer rasterizer;
  Graphics& gfx;
  Passes<BackgroundShader, ForegroundShader, UIShader> passes;
};

