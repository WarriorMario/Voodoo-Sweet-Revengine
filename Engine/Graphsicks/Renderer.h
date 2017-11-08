#pragma once
#include "Rasterizer.h"
#include "VTuple.h"
#include "VArray.h"
#include "VMove.h"
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
      grid.PlaceTriangleInCell(commands[i].prim_data, i);
      //rasterizer.RasterizeCells(grid, commands[i]);
    }
  }

  Array<Shader> commands;
};

// ****************************************************************************

class Renderer
{
  using Passes = Tuple<RenderPass<BackgroundShader>>;
  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t<I == sizeof...(Types), void>
    ApplyPasses(Tuple<Types...>&, Args&&...)
  {}
  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t < I<sizeof...(Types), void>
    ApplyPasses(Tuple<Types...>& tuple, Args&&... args)
  {
    Get<I>(tuple).Apply(args...);
    ApplyPasses<I + 1, Types...>(tuple, args...);
  }

public:
	Renderer(Graphics& gfx);

  template<typename Shader>
  void AddDrawCommand(Shader& s)
  {
    Get<Shader>(passes).commands.push_back(s);
  }
  void Render();

  ScreenGrid grid;
private:
  Rasterizer rasterizer;
  Graphics& gfx;
  Passes passes;
};

