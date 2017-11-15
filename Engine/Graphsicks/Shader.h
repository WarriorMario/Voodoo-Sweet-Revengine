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
    return Color((unsigned char)(pixel_data.u * 255.f), (unsigned char)(pixel_data.v * 255.f), 0);
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
    const float ldist = 3000.f;

    const float dx = pixel_data.x - lx;
    const float dy = pixel_data.y - ly;

    float i = 1.f - (dx * dx + dy * dy) / (ldist * ldist);
    i = Max(i, 0.f);

    Color t = const_data.texture->Sample(pixel_data.u, pixel_data.v);
    float a = t.GetA() / 255.f;
    return Color((unsigned char)(t.GetR() * i * a), (unsigned char)(t.GetG() * i  * a), (unsigned char)(t.GetB() * i * a));
  }
};

// ****************************************************************************
struct UIShader
{
  struct ConstData
  {
    Color color;
    Color* pixels;
    int width, height;

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
    int idx_y = pixel_data.v * (float)const_data.height;
    int idx_x = pixel_data.u * (float)const_data.width;
    int tex_pixel_idx = idx_x + idx_y * const_data.width;

    assert(tex_pixel_idx >= 0 && tex_pixel_idx < (const_data.width * const_data.height) &&
    "The pixel you are trying to access is out of bounds");

    Color final_pixel = const_data.color * Color(const_data.pixels[tex_pixel_idx]);
    final_pixel.ApplyAlphaTheWrongWay();

    return final_pixel;
  }
};