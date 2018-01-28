#pragma once

#include "Resource.h"
#include "Colors.h"
#include "STB/stb_image.h"
#include "Utility/ResourceAllocator.h"
#include "SIMD/SIMD.h"

// ****************************************************************************
// Converts in_pixels to Color format
// DANGER: allocates an array of Color values on the heap
static Color* ConvertPixels(stbi_uc* in_texels, int x, int y, int channels)
{
  Color* texels = ResourceAllocator::Get().Allocate<Color>(x * y);
  switch(channels)
  {
    case 1:
      // every value of every color is the greyscale value
      for(int i = 0; i < x * y * channels; ++i)
      {
        texels[i / channels] = Color(in_texels[i], in_texels[i], in_texels[i]);
      }
      break;
    case 2:
      // every value of every color is the greyscale value,
      // alpha is alpha value
      for(int i = 0; i < x * y * channels; i += channels)
      {
        texels[i / channels] = Color(in_texels[i + 1], in_texels[i], in_texels[i], in_texels[i]);
      }
      break;
    case 3:
      // copy values, alpha is default
      for(int i = 0; i < x * y * channels; i += channels)
      {
        texels[i / channels] = Color(in_texels[i], in_texels[i + 1], in_texels[i + 2]);
      }
      break;
    case 4:
      // swizzle the format from rgba (stb image) to arbg (chili)
      for(int i = 0; i < x * y * channels; i += channels)
      {
        texels[i / channels] = Color(in_texels[i + 3], in_texels[i], in_texels[i + 1], in_texels[i + 2]);
      }
      break;
    default:
      return nullptr;
  }
  return texels;
}

// ****************************************************************************
class Texture : public Resource<Texture>
{
public:
  struct Data
  {
    Data(Data&& source)
      :
      width(source.width),
      height(source.height),
      texels(source.texels)
    {
      source.texels = nullptr;
    }
    ~Data()
    {
      if(texels)
      {
        //delete[] texels;
      }
    }

    int width, height;
    Color* texels;
  };

public:
  Texture() = default;
  Texture(StringRef filename)
    : Base(filename)
  {}

  int GetWidth()
  {
	  auto& data = GetData<Texture>();
	  return data.width;
  }
  int GetHeight()
  {
	  auto& data = GetData<Texture>();
	  return data.height;
  }
  Color Sample(float u, float v)
  {
    auto& data = GetData<Texture>();
    const int tx = (int)(data.width * (u - FLT_EPSILON));
    const int ty = (int)(data.height * (v - FLT_EPSILON));
    return data.texels[ty * data.width + tx];
  }
  __m256i Sample(__m256& u8, __m256& v8)
  {
    auto& data = GetData<Texture>();
    __m256i tx8 = AVX_INT32_FROM_FLOAT(AVX_FLOAT_MUL(AVX_FLOAT_FROM1((float)data.width), AVX_FLOAT_SUB(u8, AVX_FLOAT_FROM1(FLT_EPSILON))));
    __m256i ty8 = AVX_INT32_FROM_FLOAT(AVX_FLOAT_MUL(AVX_FLOAT_FROM1((float)data.height), AVX_FLOAT_SUB(v8, AVX_FLOAT_FROM1(FLT_EPSILON))));
    __m256i index = AVX_INT32_ADD(AVX_INT32_MUL(ty8, AVX_INT32_FROM1(data.width)), tx8);
    return AVX_INT32_FROM8(data.texels[index.m256i_i32[0]].dword,
      data.texels[index.m256i_i32[1]].dword,
      data.texels[index.m256i_i32[2]].dword,
      data.texels[index.m256i_i32[3]].dword,
      data.texels[index.m256i_i32[4]].dword,
      data.texels[index.m256i_i32[5]].dword,
      data.texels[index.m256i_i32[6]].dword, 
      data.texels[index.m256i_i32[7]].dword);
  }
private:
  friend bool Load(StringRef filename, Data& data)
  {
    int channels;
    stbi_uc* texels = stbi_load(filename.data(), &data.width, &data.height, &channels, STBI_default);
    if(!texels)
    {
      return false;
    }

    data.texels = ConvertPixels(texels, data.width, data.height, channels);

    free(texels);

    return true;
  }
};