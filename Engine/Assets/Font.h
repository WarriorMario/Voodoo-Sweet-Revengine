#pragma once

#include "Resource.h"
#include "STB\stb_truetype.h"
#include "STB\stb_rect_pack.h"
#include "Utility\FileIO.h"
#include "VTraits.h"
#include "VMap.h"

/*
TODO
  load file data
  load font using stb
  get all (?) glyphs
  create font atlas
  save as texture
*/

static constexpr char BASE_CHAR[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,-=+\\/()*&^@#!_><?%~:;\"'_";
static constexpr size_t NUM_BASE_CHARS = CountOf(BASE_CHAR);
static constexpr size_t TEXT_QUAD_BUFF_SIZE = 256;
static constexpr int WHITE = 255 << 16 | 255 << 8 | 255;

struct FontAtlas
{
  ~FontAtlas()
  {
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      delete[] pixels;
    }
  }

  unsigned int* pixels[NUM_BASE_CHARS];
  unsigned int width_height[NUM_BASE_CHARS][2];
  size_t tot_pixels;
};

struct TextRenderData
{
  struct GlyphQuad
  {
    unsigned int* glyph_pixels;
    unsigned int glyph_width, glyph_height;

    int min_x, min_y, max_x, max_y;
    float min_u, min_v, max_u, max_v;
  };

  GlyphQuad text_quads[TEXT_QUAD_BUFF_SIZE];
  FontAtlas atlas;
};

// ****************************************************************************
class Font : public Resource<Font>
{
public:
  struct Data
  {
    Data(Data&& source)
      :
      font_file(source.font_file),
      stb_font(source.stb_font),
      font_atlas_map(Move(source.font_atlas_map))
    {
      source.font_file.data = nullptr;
    }
    ~Data()
    {
      if(font_file.data)
      {
        delete[] font_file.data;
      }
    }

    // file data
    File font_file;

    // the stb font
    stbtt_fontinfo stb_font;

    mutable Map<int, FontAtlas> font_atlas_map;
  };

public:
  Font(StringRef filename)
    : Base(filename)
  {}

  TextRenderData RenderLine(StringRef text, float point_size, int start_x, int start_y)
  {
    TextRenderData text_render_data;
    assert(text.size() < TEXT_QUAD_BUFF_SIZE);

    // get your data
    auto data = GetData<Font>();

    // get the font atlas, load if necessary
    auto res = data->font_atlas_map.find((int)(point_size));
    (res != data->font_atlas_map.end()) ?
      text_render_data.atlas = res->second :
      text_render_data.atlas = LoadFontAtlas(point_size);

    int cur_x = start_x, cur_y = start_y;

    // per letter
    for(int i = 0; i < text.size(); ++i)
    {
      // get the index of the char
      int idx = GetIndexInBaseChar(text[i]);

      // create a quad to be drawn

      // set the uv values
      text_render_data.text_quads[i].min_u = 0.0f;
      text_render_data.text_quads[i].min_v = 0.0f;
      text_render_data.text_quads[i].max_u = 1.0f;
      text_render_data.text_quads[i].max_v = 1.0f;

      // set min values
      text_render_data.text_quads[i].min_x = cur_x;
      text_render_data.text_quads[i].min_y = cur_y;

      // update cur_x (and cur_y in the future)
      cur_x += text_render_data.atlas.width_height[i][0];

      // set the max values
      text_render_data.text_quads[i].max_x = cur_x;
      text_render_data.text_quads[i].max_y = cur_y + text_render_data.atlas.width_height[i][1];

      // set the glyph's texture data
      text_render_data.text_quads[i].glyph_pixels = text_render_data.atlas.pixels[i];
      text_render_data.text_quads[i].glyph_width = text_render_data.atlas.width_height[i][0];
      text_render_data.text_quads[i].glyph_height = text_render_data.atlas.width_height[i][1];
    }
  }

private:
  friend bool Load(StringRef filename, Data& data)
  {
    if(LoadFileBinary(filename.data(), &data.font_file) == false)
    {
      return false;
    }

    if(stbtt_InitFont(&data.stb_font, (unsigned char*)data.font_file.data,
      stbtt_GetFontOffsetForIndex((unsigned char*)data.font_file.data, 0)))
    {
      return true; // load was succesful
    }

    return false;
  }

  FontAtlas LoadFontAtlas(int point_size)
  {
    // get your data
    auto data = GetData<Font>();
    
    // the return value
    FontAtlas font_atlas;

    // four arrays to hold the bitmap sizes per glyph
    int w_h_ox_oy[NUM_BASE_CHARS][4];
    unsigned char* mono_bitmaps[NUM_BASE_CHARS];

    // get the scale that stb_truetype can use
    float scale = stbtt_ScaleForPixelHeight(&data->stb_font, (float)point_size);

    // keep track of the amount of pixels
    size_t tot_size = 0;

    // use stb_truetype to rasterize all of the glyphs for me
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      mono_bitmaps[i] = stbtt_GetGlyphBitmap(&data->stb_font, 0.0f, scale, BASE_CHAR[i],
        &w_h_ox_oy[i][0], &w_h_ox_oy[i][1], &w_h_ox_oy[i][2], &w_h_ox_oy[i][3]);

      tot_size += w_h_ox_oy[i][0] + w_h_ox_oy[i][1];
    }

    // allocate memory for the font textures
    font_atlas.tot_pixels = tot_size;
    font_atlas.pixels[0] = new unsigned int[tot_size * sizeof(unsigned int)];

    /////////////////////////////////////////////////////////////
    // align all textures underneath each other. 
    /////////////////////////////////////////////////////////////

    // get the initial pixel
    unsigned int* p = font_atlas.pixels[0];
    // for all glyphs we'll use
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      // reset idx counter
      size_t idx_glyph_tex = 0;

      // set the appropriate font atlas values
      font_atlas.pixels[i] = p;
      font_atlas.width_height[i][0] = w_h_ox_oy[i][0];
      font_atlas.width_height[i][1] = w_h_ox_oy[i][1];

      // loop through all of the pixels
      for(int y = 0; y < w_h_ox_oy[i][1]; ++y)
      {
        for(int x = 0; x < w_h_ox_oy[i][0]; ++x, ++idx_glyph_tex)
        {
          // per pixel, calculate the final value (mono value for alpha, white for the rest)
          *p = mono_bitmaps[i][idx_glyph_tex] << 24 | WHITE;

          // increment the pointer after the pixel to move on to the next one
          p++;
        }
      }
    }
    
    // place the font atlas in the map
    data->font_atlas_map[point_size] = font_atlas;

    // return the filled out font atlas
    return font_atlas;
  }


  // get the char in the index
  /* 
  TODO (Floris) optimize this, might become a bottleneck. probably not though.
  */
  inline int GetIndexInBaseChar(char c)
  {
    int i = 0;
    while(i < NUM_BASE_CHARS)
    {
      if(c == BASE_CHAR[i])
      {
        return i;
      }
      i++;
    }
    assert(false && "character not found while parsing string. plz gib more base chars or stop using stupid glyps");
    return 0;
  }
};