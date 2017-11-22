#pragma once
#include "Colors.h"
#include "Utility.h"
#include "Assets\Assets.h"
#include "VArray.h"
#include "Vec2.h"
#include "Utility\SIMD.h"
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
public:
  struct ConstData
  {
    Color color;
    float u10, v10, u20, v20;
  };

  struct PrimData
  {
    float x, y;
    float u, v;
  };

  struct DataSIMDBuffer
  {
    void Add(BackgroundShader& s)
    {}
    void Clear()
    {}
    void GetTriangle(Vec2* points, size_t index)
    {}
    size_t size;
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

  __forceinline void Shade(const InterpData& interp_data, Color& pixel)
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
  __forceinline void ShadeSIMD(const __m256& t1, const __m256& t2, const __m256i& mask, int x, int y, Color* pixel)
  {}
  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }
  static constexpr bool SIMD = false;
private:
  ConstData const_data;
  PrimData prim_data[3];
};

// ****************************************************************************
struct ForegroundShader
{
private:
public:
  struct ConstData
  {
    Color color;
    Texture* texture;
    float u10, v10, u20, v20;
    float nx10, ny10, nx20, ny20;
  };

  struct PrimData
  {
    float x, y;
    float u, v;
    float nx, ny;
  };

  struct DataSIMDBuffer
  {
    void Add(ForegroundShader& s)
    {}
    void Clear()
    {}
    void GetTriangle(Vec2* points, size_t index)
    {}
    size_t size;
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

    const_data.u10 = prim_data[1].u - prim_data[0].u;
    const_data.v10 = prim_data[1].v - prim_data[0].v;
    const_data.nx10 = prim_data[1].nx - prim_data[0].nx;
    const_data.ny10 = prim_data[1].ny - prim_data[0].ny;
    const_data.u20 = prim_data[2].u - prim_data[0].u;
    const_data.v20 = prim_data[2].v - prim_data[0].v;
    const_data.nx10 = prim_data[2].nx - prim_data[0].nx;
    const_data.ny20 = prim_data[2].ny - prim_data[0].ny;
  }

  __forceinline void Shade(const InterpData& interp_data, Color& pixel)
  {
    /////////////////////////////////////
    // Interpolation
    /////////////////////////////////////

    // for convenience
    const PrimData& p0 = prim_data[0];
    const PrimData& p1 = prim_data[1];
    const PrimData& p2 = prim_data[2];

    // only requires t1 and t2
    float u = p0.u + const_data.u10 * interp_data.t1 + const_data.u20 * interp_data.t2;// Texture power of 2 to save some more bits
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

  __forceinline void ShadeSIMD(const __m256& t1, const __m256& t2, const __m256i& mask, int x, int y, Color* pixel)
  {
    /////////////////////////////////////
    // Interpolation
    /////////////////////////////////////

    //__m256i inverseMaskTest = _mm256_xor_si256(mask, _mm256_set1_epi32(0xffffffff));
    //*((__m256i*)pixel) = _mm256_add_epi32(_mm256_and_si256(inverseMaskTest, _mm256_set1_epi32(pixel->dword)), _mm256_and_si256(mask, _mm256_cvtps_epi32(AVXMULPS(t2, AVXSET1PS(255.0f)))));
    //return;
    //// for convenience
    //const PrimData& p0 = prim_data[0];
    //const PrimData& p1 = prim_data[1];
    //const PrimData& p2 = prim_data[2];
    ////
    ////// only requires t1 and t2
    //float u = p0.u + const_data.u10 * t1.m256_f32[3] + const_data.u20 * t2.m256_f32[3];
    //float v = p0.v + const_data.v10 * t1.m256_f32[3] + const_data.v20 * t2.m256_f32[3];
    __m256 p0u8 = AVX_FLOAT_FROM1(prim_data[0].u);
    __m256 p0v8 = AVX_FLOAT_FROM1(prim_data[0].v);
    __m256 p10u8 = AVX_FLOAT_FROM1(const_data.u10);
    __m256 p20u8 = AVX_FLOAT_FROM1(const_data.u20);
    __m256 p10v8 = AVX_FLOAT_FROM1(const_data.v10);
    __m256 p20v8 = AVX_FLOAT_FROM1(const_data.v20);
    __m256 u8 = AVX_FLOAT_ADD(AVX_FLOAT_ADD(p0u8, AVX_FLOAT_MUL(p10u8, t1)), AVX_FLOAT_MUL(p20u8, t2));
    __m256 v8 = AVX_FLOAT_ADD(AVX_FLOAT_ADD(p0v8, AVX_FLOAT_MUL(p10v8, t1)), AVX_FLOAT_MUL(p20v8, t2));

    //Color t = const_data.texture->Sample(u, v);
    //float a = t.GetA() / 255.f;
    //pixel = Color(t.GetR() * i * a, t.GetG() * i  * a, t.GetB() * i * a);
    
    __m256i inverseMask = AVX_INT32_XOR(mask, AVX_INT32_FROM1(0xffffffff));
    
    //__m256i inverseMaskTest = _mm256_xor_si256(mask, _mm256_set1_epi32(0xffffffff));
    //*((__m256i*)pixel) = _mm256_add_epi32(_mm256_and_si256(inverseMaskTest, _mm256_set1_epi32(pixel->dword)), _mm256_and_si256(mask, _mm256_cvtps_epi32(AVXMULPS(v8, AVXSET1PS(255.0f)))));
    //return;
    //*((__m256i*)pixel) = _mm256_add_epi32(_mm256_and_si256(inverseMask, ), _mm256_and_si256(mask, _mm256_set1_epi32(-1)));
    *((__m256i*)pixel) = AVX_INT32_ADD(AVX_INT32_AND(inverseMask, *((__m256i*)pixel)), AVX_INT32_AND(mask, const_data.texture->Sample(u8, v8)));


    //((__m256i*)pixel)->m256i_i32[0] = const_data.texture->Sample(u8.m256_f32[0], v8.m256_f32[0]).dword;
    //((__m256i*)pixel)->m256i_i32[1] = const_data.texture->Sample(u8.m256_f32[1], v8.m256_f32[1]).dword;
    //((__m256i*)pixel)->m256i_i32[2] = const_data.texture->Sample(u8.m256_f32[2], v8.m256_f32[2]).dword;
    //((__m256i*)pixel)->m256i_i32[3] = const_data.texture->Sample(u8.m256_f32[3], v8.m256_f32[3]).dword;
    //((__m256i*)pixel)->m256i_i32[4] = const_data.texture->Sample(u8.m256_f32[4], v8.m256_f32[4]).dword;
    //((__m256i*)pixel)->m256i_i32[5] = const_data.texture->Sample(u8.m256_f32[5], v8.m256_f32[5]).dword;
    //((__m256i*)pixel)->m256i_i32[6] = const_data.texture->Sample(u8.m256_f32[6], v8.m256_f32[6]).dword;
    //((__m256i*)pixel)->m256i_i32[7] = const_data.texture->Sample(u8.m256_f32[7], v8.m256_f32[7]).dword;
    //*((__m256i*)pixel) = const_data.texture->Sample(u8, v8);
  }

  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

  static constexpr bool SIMD = true;
private:
  ConstData const_data;
  PrimData prim_data[3];
};

// ****************************************************************************
struct UIShader
{
private:
public:
  struct ConstData
  {

    Color color;
    unsigned char* alpha_values;
    int width, height;
    float u10, v10, u20, v20;
  };

  struct PrimData
  {
    float x, y;
    float u, v;
  };
  struct DataSIMDBuffer
  {
    void Add(UIShader& s)
    {}
    void Clear()
    {}
    void GetTriangle(Vec2* points, size_t index)
    {}
    size_t size;
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
  __forceinline void Shade(const InterpData& interp_data, Color& pixel)
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
      ((((((cur_pixel >> 16) & 0xff) * origin_alpha) + (new_pixel_alpha * ((const_color >> 16) & 0xff))) >> 8) << 16) | // r channel alpha blended
      ((((((cur_pixel >> 8 )& 0xff) * origin_alpha) + (new_pixel_alpha * ((const_color >> 8) & 0xff))) >> 8) << 8) |  // g channel alpha blended
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
  __forceinline void ShadeSIMD(const __m256& t1, const __m256& t2, const __m256i& mask, int x, int y, Color* pixel)
  {

    __m256 p0u8 = AVX_FLOAT_FROM1(prim_data[0].u);
    __m256 p0v8 = AVX_FLOAT_FROM1(prim_data[0].v);
    __m256 p10u8 = AVX_FLOAT_FROM1(const_data.u10);
    __m256 p20u8 = AVX_FLOAT_FROM1(const_data.u20);
    __m256 p10v8 = AVX_FLOAT_FROM1(const_data.v10);
    __m256 p20v8 = AVX_FLOAT_FROM1(const_data.v20);
    __m256 u8 = AVX_FLOAT_ADD(AVX_FLOAT_ADD(p0u8, AVX_FLOAT_MUL(p10u8, t1)), AVX_FLOAT_MUL(p20u8, t2));
    __m256 v8 = AVX_FLOAT_ADD(AVX_FLOAT_ADD(p0v8, AVX_FLOAT_MUL(p10v8, t1)), AVX_FLOAT_MUL(p20v8, t2));
    
    __m256i tx8 = AVX_INT32_FROM_FLOAT(AVX_FLOAT_MUL(AVX_FLOAT_FROM1((float)const_data.width), AVX_FLOAT_SUB(u8, AVX_FLOAT_FROM1(FLT_EPSILON))));
    __m256i ty8 = AVX_INT32_FROM_FLOAT(AVX_FLOAT_MUL(AVX_FLOAT_FROM1((float)const_data.height), AVX_FLOAT_SUB(v8, AVX_FLOAT_FROM1(FLT_EPSILON))));
    __m256i index = AVX_INT32_ADD(AVX_INT32_MUL(ty8, AVX_INT32_FROM1(const_data.width)), tx8);
    __m256i alphas = AVX_INT32_FROM8((int)const_data.alpha_values[index.m256i_i32[0]],
      (int)const_data.alpha_values[index.m256i_i32[1]],
      (int)const_data.alpha_values[index.m256i_i32[2]],
      (int)const_data.alpha_values[index.m256i_i32[3]],
      (int)const_data.alpha_values[index.m256i_i32[4]],
      (int)const_data.alpha_values[index.m256i_i32[5]],
      (int)const_data.alpha_values[index.m256i_i32[6]],
      (int)const_data.alpha_values[index.m256i_i32[7]]);
    __m256i inverted_alpha = AVX_INT32_SUB(AVX_INT32_FROM1(255),alphas);
    
    __m256i dest = *(__m256i*) pixel;
    __m256i colour = AVX_INT32_FROM1(const_data.color.dword);
    __m256i alpha = AVX_INT32_SHIFT_LEFT(alphas, 24);
    __m256i red0 = AVX_INT32_MUL(AVX_INT32_AND(AVX_INT32_SHIFT_RIGHT(dest, 16), AVX_INT32_FROM1(0xFF)), inverted_alpha);
    __m256i red1 = AVX_INT32_MUL(AVX_INT32_AND(AVX_INT32_SHIFT_RIGHT(colour, 16), AVX_INT32_FROM1(0xFF)), alpha);
    __m256i red = AVX_INT32_SHIFT_LEFT(AVX_INT32_SHIFT_RIGHT(AVX_INT32_ADD(red0, red1), 8), 16);
    //((((((cur_pixel >> 16) & 0xff) * origin_alpha) + (new_pixel_alpha * ((const_color >> 16) & 0xff))) >> 8) << 16) | // r channel alpha blended
    __m256i green0 = AVX_INT32_MUL(AVX_INT32_AND(AVX_INT32_SHIFT_RIGHT(dest, 16), AVX_INT32_FROM1(0xFF)), inverted_alpha);
    __m256i green1 = AVX_INT32_MUL(AVX_INT32_AND(AVX_INT32_SHIFT_RIGHT(colour, 8), AVX_INT32_FROM1(0xFF)), alpha);
    __m256i green = AVX_INT32_SHIFT_LEFT(AVX_INT32_SHIFT_RIGHT(AVX_INT32_ADD(green0, green1), 8), 8);
    __m256i blue = AVX_INT32_SHIFT_RIGHT(AVX_INT32_ADD(AVX_INT32_MUL(AVX_INT32_AND(dest, AVX_INT32_FROM1(0xFF)), inverted_alpha), AVX_INT32_MUL(AVX_INT32_AND(colour, AVX_INT32_FROM1(0xFF)), alpha)),8);
    *(__m256i*) pixel = alphas;// _mm256_or_si256(_mm256_or_si256(_mm256_or_si256(red, green), blue), alphas);
  }
  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

  static constexpr bool SIMD = false;
private:
  ConstData const_data;
  PrimData prim_data[3];
};


struct UIShaderSimple
{

private:

public:
  struct ConstData
  {
    Color color;
  };
  struct PrimData
  {
    float x, y;
    float u, v;
  };
  struct DataSIMDBuffer
  {
    void Add(UIShaderSimple shader)
    {
      ++size;
      color.push_back(shader.const_data.color);
      for(int i = 0; i < 3; ++i)
      {
        x.push_back(shader.prim_data[i].x);
        y.push_back(shader.prim_data[i].y);
        u.push_back(shader.prim_data[i].u);
        v.push_back(shader.prim_data[i].v);
      }
    }
    void GetTriangle(Vec2* points, size_t index)
    {
      points[0] = Vec2(x[index * 3], y[index * 3]);
      points[1] = Vec2(x[index * 3 + 1], y[index * 3 + 1]);
      points[2] = Vec2(x[index * 3 + 2], y[index * 3 + 2]);
    }
    void Clear()
    {
      x.clear();
      y.clear();
      u.clear();
      v.clear();
      size = 0;
    }
    Array<Color> color;
    Array<float> x;
    Array<float> y;
    Array<float> u;
    Array<float> v;
    size_t size = 0;;
  };

  UIShaderSimple(
    const PrimData& vertex_0,
    const PrimData& vertex_1,
    const PrimData& vertex_2,
    const Color& color)
  {
    prim_data[0] = vertex_0;
    prim_data[1] = vertex_1;
    prim_data[2] = vertex_2;

    const_data.color = color;
  }
  __forceinline void Shade(const InterpData& interp_data, Color& pixel)
  {
    pixel = const_data.color;
  }
  __forceinline void ShadeSIMD(const __m256& t1, const __m256& t2, const __m256i& mask, int x, int y, Color* pixel)
  {}
  const PrimData* GetPrimData()
  {
    return &prim_data[0];
  }

  static constexpr bool SIMD = false;
private:
  ConstData const_data;
  PrimData prim_data[3];
};