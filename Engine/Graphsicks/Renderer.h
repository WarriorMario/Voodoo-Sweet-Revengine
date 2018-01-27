#pragma once
#include "Rasterizer.h"
#include "VTuple.h"
#include "VArray.h"
#include "VTraits.h"
#include "Shader.h"
#include "Gameplay\Camera.h"

class Graphics;
class Player;

// ****************************************************************************
template<typename Shader>
class RenderPass
{
public:
  void Apply(ScreenGrid& grid, Rasterizer& rasterizer, Camera& camera_transform)
  {
    Array<Shader> commands_cpy = commands;

    for(int i = 0; i < commands_cpy.size(); ++i)
    {
      auto pd = commands_cpy[i].GetPrimData();

      if(Shader::IS_ABSOLUTE == false)
      {
        for(int iVec = 0; iVec < 3; ++iVec)
        {
          pd[iVec].x += camera_transform.offset.x;
          pd[iVec].y += camera_transform.offset.y;
        }
      }

      Vec2 points[3] = {
      (Vec2(pd[0].x, pd[0].y)),
      (Vec2(pd[1].x, pd[1].y)),
      (Vec2(pd[2].x, pd[2].y))
      };
      
      grid.PlaceTriangleInCell(points, i);
    }
    if(commands_cpy.size() != 0)
    {
      rasterizer.RasterizeCells(grid, commands_cpy, false);
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
  struct PassClearer
  {
    template<typename T>
    void operator()(T& pass)
    {
      pass.commands.clear();
    }
  };

public:
  Renderer();

  template<typename Shader>
  static void AddSharedDrawCommand(Shader& s)
  {
    Get<RenderPass<Shader>>(shared_passes).commands.push_back(s);
  }
  template<typename Shader>
  void AddDrawCommand(Shader& s)
  {
    Get<RenderPass<Shader>>(passes).commands.push_back(s);
  }

  void AdjustCamera(const Array<Player*>& players);
  void Render(Color* target);
  void Reset();

  ScreenGrid grid;
  Camera camera;
  
private:
  Rasterizer rasterizer;

  static Passes<BackgroundShader, ForegroundShader, UIShader, UIShaderSimple> shared_passes;
  Passes<BackgroundShader, ForegroundShader, UIShader, UIShaderSimple> passes;
};

