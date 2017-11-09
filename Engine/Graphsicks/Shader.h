#pragma once
#include "Colors.h"
#include "Vec2.h"


// ****************************************************************************
struct BackgroundShader
{
  struct ConstData
  {
    Color color;
  };
  struct PixelData
  {
    float x, y;
    float u, v;
  };

  ConstData const_data;
  Vec2 prim_data[3];
};
struct ForegroundShader
{
  struct ConstData
  {
	  Color color;
  };
  struct PixelData
  {
    float x, y, z;
    float u, v;
    float nx, ny, nz;
  };

  ConstData const_data;
  Vec2 prim_data[3];
};
// ****************************************************************************
template<typename T>
class ShadeOperation
{
public:
	using PixelData = typename T::PixelData;
	using ConstData = typename T::ConstData;
	Color operator()(PixelData&, ConstData&);
};

//template<>
//Color ShadeOperation<BackgroundShader>::operator()(PixelData& pixel_data, ConstData& const_data);