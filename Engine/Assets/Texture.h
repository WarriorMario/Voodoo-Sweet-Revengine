#pragma once

#include "Resource.h"
#include "Colors.h"
#include "STB/stb_image.h"

// ****************************************************************************
// Converts in_pixels to Color format
// DANGER: allocates an array of Color values on the heap
static Color* ConvertPixels(stbi_uc* in_texels, int x, int y, int channels)
{
  Color* texels = new Color[x * y];
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
        delete[] texels;
      }
    }

    int width, height;
    Color* texels;
  };

public:
  Texture(StringRef filename)
    : Base(filename)
  {}

  Color Sample(float u, float v)
  {
    auto& data = GetData<Texture>();
    const int tx = (int)(data.width * (u - FLT_EPSILON));
    const int ty = (int)(data.height * (v - FLT_EPSILON));
    return data.texels[ty * data.width + tx];
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