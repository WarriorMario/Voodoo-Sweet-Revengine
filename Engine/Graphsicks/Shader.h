#pragma once
#include "Colors.h"
#include "Utility.h"
#include "Lib\FreeType\Headers\ft2build.h"


// ****************************************************************************
// To create a new shader you can declare it as a struct which implements:
// - A public member struct ConstData.
// - A public member struct PixelData, which contains at least:
//   * Two floats x and y.
// - A public variable ConstData const_data.
// - A public array variable PixelData prim_data[3].
// - A public function PixelData Interpolate(float t0, float t1, float t2),
//   which interpolates each point in prim_data.
// - A public function Color Shade(const PixelData& pixel_data), which returns
//   the final pixel color using the passed pixel data.

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
  PixelData prim_data[3];
  
  PixelData Interpolate(float t0, float t1, float t2)
  {
    PixelData res;
    const auto& p0 = prim_data[0];
    const auto& p1 = prim_data[1];
    const auto& p2 = prim_data[2];
    res.x = p0.x * t0 + p1.x * t1 + p2.x * t2;
    res.y = p0.y * t0 + p1.y * t1 + p2.y * t2;
    res.u = p0.u * t0 + p1.u * t1 + p2.u * t2;
    res.v = p0.v * t0 + p1.v * t1 + p2.v * t2;
    return res;
  }
  Color Shade(const PixelData& pixel_data)
  {
    return Color(pixel_data.u * 255.f, pixel_data.v * 255.f, 0);
  }
};

// ****************************************************************************
struct ForegroundShader
{
  struct ConstData
  {
	  Color color;
  };
  struct PixelData
  {
    float x, y;
    float u, v;
    float nx, ny;
  };

  ConstData const_data;
  PixelData prim_data[3];

  PixelData Interpolate(float t0, float t1, float t2)
  {
    PixelData res;
    const auto& p0 = prim_data[0];
    const auto& p1 = prim_data[1];
    const auto& p2 = prim_data[2];
    res.x = p0.x * t0 + p1.x * t1 + p2.x * t2;
    res.y = p0.y * t0 + p1.y * t1 + p2.y * t2;
    res.u = p0.u * t0 + p1.u * t1 + p2.u * t2;
    res.v = p0.v * t0 + p1.v * t1 + p2.v * t2;
    res.nx = p0.nx * t0 + p1.nx * t1 + p2.nx * t2;
    res.ny = p0.ny * t0 + p1.ny * t1 + p2.ny * t2;
    return res;
  }
  Color Shade(const PixelData& pixel_data)
  {
    const float lx = 200.f;
    const float ly = 200.f;
    const float ldist = 300.f;

    const float dx = pixel_data.x - lx;
    const float dy = pixel_data.y - ly;

    float i = 1.f - (dx * dx + dy * dy) / (ldist * ldist);
    i = Max(i, 0.f);
    return Color(i * 255.f, i * 255.f, i * 255.f);
  }
};

// ****************************************************************************
struct TextShader
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
	PixelData prim_data[3];

	PixelData Interpolate(float t0, float t1, float t2)
	{
		PixelData res;
		const auto& p0 = prim_data[0];
		const auto& p1 = prim_data[1];
		const auto& p2 = prim_data[2];
		res.x = p0.x * t0 + p1.x * t1 + p2.x * t2;
		res.y = p0.y * t0 + p1.y * t1 + p2.y * t2;
		res.u = p0.u * t0 + p1.u * t1 + p2.u * t2;
		res.v = p0.v * t0 + p1.v * t1 + p2.v * t2;
		return res;
	}
	Color Shade(const PixelData& pixel_data)
	{
		const float lx = 200.f;
		const float ly = 200.f;
		const float ldist = 300.f;

		const float dx = pixel_data.x - lx;
		const float dy = pixel_data.y - ly;

		float i = 1.f - (dx * dx + dy * dy) / (ldist * ldist);
		i = Max(i, 0.f);
		return Color(i * 255.f, i * 255.f, i * 255.f);
	}
};