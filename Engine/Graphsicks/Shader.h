#pragma once
#include "Colors.h"
#include "Utility.h"
#include "Assets\Assets.h"

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

struct InterpData
{
  float t0, t1, t2;
  int x, y;
};

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

  void Interpolate(const InterpData& interp_data, PixelData& res)
  {
    const auto& p0 = prim_data[0];
    const auto& p1 = prim_data[1];
    const auto& p2 = prim_data[2];
    res.x = interp_data.x;
    res.y = interp_data.y;
    res.u = p0.u * interp_data.t0 + p1.u * interp_data.t1 + p2.u * interp_data.t2;
    res.v = p0.v * interp_data.t0 + p1.v * interp_data.t1 + p2.v * interp_data.t2;
  }

  void Shade(const PixelData& pixel_data, Color& pixel)
  {
    pixel = Color((unsigned char)(pixel_data.u * 255.f), (unsigned char)(pixel_data.v * 255.f), 0);
  }
};

// ****************************************************************************
struct ForegroundShader
{
  struct ConstData
  {
    Color color;
    Texture* texture;
  };

  struct PixelData
  {
    float x, y;
    float u, v;
    float nx, ny;
  };

  ConstData const_data;
  PixelData prim_data[3];

  void Interpolate(const InterpData& interp_data, PixelData& res)
  {
    const auto& p0 = prim_data[0];
    const auto& p1 = prim_data[1];
    const auto& p2 = prim_data[2];
    res.x = interp_data.x;
    res.y = interp_data.y;
    res.u = p0.u * interp_data.t0 + p1.u * interp_data.t1 + p2.u * interp_data.t2;
    res.v = p0.v * interp_data.t0 + p1.v * interp_data.t1 + p2.v * interp_data.t2;
    res.nx = p0.nx * interp_data.t0 + p1.nx * interp_data.t1 + p2.nx * interp_data.t2;
    res.ny = p0.ny * interp_data.t0 + p1.ny * interp_data.t1 + p2.ny * interp_data.t2;
  }

  void Shade(const PixelData& pixel_data, Color& pixel)
  {
    const float lx = 200.f;
    const float ly = 200.f;
    const float ldist = 3000.f;

    const float dx = pixel_data.x - lx;
    const float dy = pixel_data.y - ly;

    float i = 1.f - (dx * dx + dy * dy) / (ldist * ldist);
    i = Max(i, 0.f);

    Color t = const_data.texture->Sample(pixel_data.u, pixel_data.v);
    float a = t.GetA() / 255.f;
    pixel = Color((unsigned char)(t.GetR() * i * a), (unsigned char)(t.GetG() * i  * a), (unsigned char)(t.GetB() * i * a));
  }
};

// ****************************************************************************
struct UIShader
{
  struct ConstData
  {
    Color color;
    unsigned char* alpha_values;
    int width, height;
  };

  struct PixelData
  {
    float x, y;
    float u, v;
  };

  ConstData const_data;
  PixelData prim_data[3];

  void Interpolate(const InterpData& interp_data, PixelData& res)
  {
    const auto& p0 = prim_data[0];
    const auto& p1 = prim_data[1];
    const auto& p2 = prim_data[2];
    res.x = interp_data.x;
    res.y = interp_data.y;
    res.u = p0.u * interp_data.t0 + p1.u * interp_data.t1 + p2.u * interp_data.t2;
    res.v = p0.v * interp_data.t0 + p1.v * interp_data.t1 + p2.v * interp_data.t2;
  }

  void Shade(const PixelData& pixel_data, Color& pixel)
  {
    int idx_y = pixel_data.v * (float)const_data.height;
    int idx_x = pixel_data.u * (float)const_data.width;
    int tex_pixel_idx = idx_x + idx_y * const_data.width;

    assert(tex_pixel_idx >= 0 && tex_pixel_idx < (const_data.width * const_data.height) &&
    "The pixel you are trying to access is out of bounds");

    Color final_pixel = ((*(int*)&const_data.color) & 0x00ffffff) |
      ((int)const_data.alpha_values[tex_pixel_idx] << 24);

    final_pixel.ApplyAlphaTheRightWay(pixel);
    pixel = final_pixel;
  }
};