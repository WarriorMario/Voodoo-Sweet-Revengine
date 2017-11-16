#pragma once
#include "Colors.h"
#include "Utility.h"
#include "Assets\Assets.h"


//////////// OBSOLETE TODO (Floris) unobsoletify this
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
  float t1, t2;
  int x, y;
};

struct BackgroundShader
{
private:
  struct ConstData
  {
    Color color;
    float u10, v10, u20, v20;
  };

public:
  struct PrimData
  {
    float x, y;
    float u, v;
  };

  BackgroundShader(
    const PrimData& vertex_0,
    const PrimData& vertex_1,
    const PrimData& vertex_2,
    const Color& color)
  {
    prim_data[0] = vertex_0;
    prim_data[1] = vertex_1;
    prim_data[2] = vertex_2;
    const_data.color = color;

    const_data.u10 = prim_data[1].u - prim_data[0].u;
    const_data.v10 = prim_data[1].v - prim_data[0].v;
    const_data.u20 = prim_data[2].u - prim_data[0].u;
    const_data.v20 = prim_data[2].v - prim_data[0].v;
  }

  void Shade(const InterpData& interp_data, Color& pixel)
  {
    /////////////////////////////////////
    // Interpolation
    /////////////////////////////////////

    const PrimData& p0 = prim_data[0];
    const PrimData& p1 = prim_data[1];
    const PrimData& p2 = prim_data[2];
    float u = p0.u + const_data.u10 * interp_data.t1 + const_data.u20 * interp_data.t2;
    float v = p0.v + const_data.v10 * interp_data.t1 + const_data.v20 * interp_data.t2;

    /////////////////////////////////////
    // Shading
    /////////////////////////////////////
    pixel = Color((unsigned char)(u * 255.f), (unsigned char)(v * 255.f), 0);
  }

  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

private:
  ConstData const_data;
  PrimData prim_data[3];
};

// ****************************************************************************
struct ForegroundShader
{
private:
  struct ConstData
  {
    Color color;
    Texture* texture;
    float u10, v10, u20, v20;
    float nx10, ny10, nx20, ny20;
  };

public:
  struct PrimData
  {
    float x, y;
    float u, v;
    float nx, ny;
  };

  ForegroundShader(
    const PrimData& vertex_0,
    const PrimData& vertex_1, 
    const PrimData& vertex_2,
    const Color& color, Texture* texture)
  {
    prim_data[0] = vertex_0;
    prim_data[1] = vertex_1;
    prim_data[2] = vertex_2;
    const_data.color = color;
    const_data.texture = texture;

    const_data.u10  = prim_data[1].u  - prim_data[0].u;
    const_data.v10  = prim_data[1].v  - prim_data[0].v;
    const_data.nx10 = prim_data[1].nx - prim_data[0].nx;
    const_data.ny10 = prim_data[1].ny - prim_data[0].ny;
    const_data.u20  = prim_data[2].u  - prim_data[0].u;
    const_data.v20  = prim_data[2].v  - prim_data[0].v;
    const_data.nx10 = prim_data[2].nx - prim_data[0].nx;
    const_data.ny20 = prim_data[2].ny - prim_data[0].ny;
  }

  void Shade(const InterpData& interp_data, Color& pixel)
  {
    /////////////////////////////////////
    // Interpolation
    /////////////////////////////////////

    // for convenience
    const PrimData& p0 = prim_data[0];
    const PrimData& p1 = prim_data[1];
    const PrimData& p2 = prim_data[2];

    // only requires t1 and t2
    float u = p0.u + const_data.u10 * interp_data.t1 + const_data.u20 * interp_data.t2;
    float v = p0.v + const_data.v10 * interp_data.t1 + const_data.v20 * interp_data.t2;
    float nx = p0.nx + const_data.nx10 * interp_data.t1 + const_data.nx20 * interp_data.t2;
    float ny = p0.ny + const_data.ny10 * interp_data.t1 + const_data.ny20 * interp_data.t2;

    /////////////////////////////////////
    // Shading
    /////////////////////////////////////

    const float lx = 200.f;
    const float ly = 200.f;
    const float ldist = 3000.f;

    const float dx = interp_data.x - lx;
    const float dy = interp_data.y - ly;

    float i = 1.f - (dx * dx + dy * dy) / (ldist * ldist);
    i = Max(i, 0.f);

    Color t = const_data.texture->Sample(u, v);
    float a = t.GetA() / 255.f;
    pixel = Color(t.GetR() * i * a, t.GetG() * i  * a, t.GetB() * i * a);
  }

  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

private:
  ConstData const_data;
  PrimData prim_data[3];
};

// ****************************************************************************
struct UIShader
{
private:
  struct ConstData
  {

    Color color;
    unsigned char* alpha_values;
    int width, height;
    float u10, v10, u20, v20;
  };

public:
  struct PrimData
  {
    float x, y;
    float u, v;
  };

  UIShader(
    const PrimData& vertex_0,
    const PrimData& vertex_1,
    const PrimData& vertex_2,
    unsigned char* alpha_values,
    int width, int height,
    const Color& color)
  {
    prim_data[0] = vertex_0;
    prim_data[1] = vertex_1;
    prim_data[2] = vertex_2;

    const_data.color = color;
    const_data.alpha_values = alpha_values;
    const_data.width = width;
    const_data.height = height;

    const_data.u10 = prim_data[1].u - prim_data[0].u;
    const_data.v10 = prim_data[1].v - prim_data[0].v;
    const_data.u20 = prim_data[2].u - prim_data[0].u;
    const_data.v20 = prim_data[2].v - prim_data[0].v;
  }
  void Shade(const InterpData& interp_data, Color& pixel)
  {
    /////////////////////////////////////
    // Interpolation
    /////////////////////////////////////

    // for convenience
    const PrimData& p0 = prim_data[0];
    const PrimData& p1 = prim_data[1];
    const PrimData& p2 = prim_data[2];

    // only requires t1 and t2
    float u = p0.u + const_data.u10 * interp_data.t1 + const_data.u20 * interp_data.t2;
    float v = p0.v + const_data.v10 * interp_data.t1 + const_data.v20 * interp_data.t2;

    /////////////////////////////////////
    // Shading
    /////////////////////////////////////
#if 1
    // get texel's index
    int idx_x = u * (float)const_data.width;
    int idx_y = v * (float)const_data.height;
    int tex_pixel_idx = idx_x + idx_y * const_data.width;

    // simple assert
    assert(tex_pixel_idx >= 0 && tex_pixel_idx < (const_data.width * const_data.height) &&
      "The pixel you are trying to access is out of bounds");

    // alpha value of new pixel
    unsigned char new_pixel_alpha = (int)const_data.alpha_values[tex_pixel_idx];

    // inverse alpha value
    unsigned char origin_alpha = ~new_pixel_alpha;

    // get the color values
    int const_color = (*(int*)&const_data.color);

    // pointer cast pixel to int
    int cur_pixel = *(int*)&pixel;

    // apply alpha on each channel, assign alpha to the pixel
    pixel = (new_pixel_alpha << 24) | // mask in alpha channel
      (((((cur_pixel >> 16 & 0xff) * origin_alpha) + (new_pixel_alpha * (const_color >> 16 & 0xff))) >> 8) << 16) | // r channel alpha blended
      (((((cur_pixel >> 8 & 0xff) * origin_alpha) + (new_pixel_alpha * (const_color >> 8 & 0xff))) >> 8) << 8) |  // g channel alpha blended
      (((((cur_pixel & 0xff) * origin_alpha) + (new_pixel_alpha * (const_color & 0xff))) >> 8));        // b channel alpha blended;

    //// might be faster when SIMDified
    //int rb = (((cur_pixel & 0x00ff00ff) * origin_alpha) + ((const_color & 0x00ff00ff) * new_pixel_alpha)) & 0xff00ff00;
    //int g  = (((cur_pixel & 0x0000ff00) * origin_alpha) + ((const_color & 0x0000ff00) * new_pixel_alpha)) & 0x00ff0000;
    //pixel  = (cur_pixel & 0xff000000) | ((rb | g) >> 8) + 0x00010101;
#else
    int idx_y = pixel_data.v * (float)const_data.height;
    int idx_x = pixel_data.u * (float)const_data.width;
    int tex_pixel_idx = idx_x + idx_y * const_data.width;

    assert(tex_pixel_idx >= 0 && tex_pixel_idx < (const_data.width * const_data.height) &&
      "The pixel you are trying to access is out of bounds");

    Color final_pixel = (*(int*)&const_data.color & 0x00ffffff) |
      ((int)const_data.alpha_values[tex_pixel_idx] << 24);
    final_pixel.ApplyAlphaTheRightWay(pixel);

    pixel = final_pixel;
#endif
  }

  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

  private:
    ConstData const_data;
    PrimData prim_data[3];
};