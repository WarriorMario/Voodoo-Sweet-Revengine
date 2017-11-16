#pragma once
#include "Resource.h"
#include "STB\stb_truetype.h"
#include "STB\stb_rect_pack.h"
#include "Utility\FileIO.h"
#include "VTraits.h"
#include "VMap.h"

static constexpr char BASE_CHAR[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,-=+\\/()*&^@#!_><?%~:;\"'_";
static constexpr size_t NUM_BASE_CHARS = CountOf(BASE_CHAR);
static constexpr size_t TEXT_QUAD_BUFF_SIZE = 256;
static constexpr int WHITE = 255 << 16 | 255 << 8 | 255;

struct FontAtlas
{
  FontAtlas()
  {
    tot_alpha_values = 0;
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      alpha_values[i] = nullptr;
    }
  }
  void Clear()
  {
    delete[] alpha_values[0];
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      alpha_values[i] = nullptr;
    }
  }

  unsigned char* alpha_values[NUM_BASE_CHARS];
  int w_h_ox_oy[NUM_BASE_CHARS][4];
  size_t tot_alpha_values;
};

struct TextRenderData
{
  struct GlyphQuad
  {
    unsigned char* glyph_alpha_values;
    unsigned int glyph_width, glyph_height;

    int min_x, min_y, max_x, max_y;
    float min_u, min_v, max_u, max_v;
  };

  GlyphQuad text_quads[TEXT_QUAD_BUFF_SIZE];
  FontAtlas atlas;
  size_t num_quads;
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

      std::map<int, FontAtlas>::iterator it;

      // free the data of everything in the map
      for(it = font_atlas_map.begin(); it != font_atlas_map.end(); it++)
      {
        it->second.Clear();
      }

      // clear the map itself
      font_atlas_map.clear();
    }

    // file data
    File font_file;

    // the stb font
    stbtt_fontinfo stb_font;

    // map of all the font atlasses we currently have
    mutable Map<int, FontAtlas> font_atlas_map;
  };

public:
  Font(StringRef filename)
    : Base(filename)
  {}

  TextRenderData RenderLine(StringRef text, int point_size, int start_x, int start_y, int space_size = 30)
  {
    TextRenderData text_render_data;
    assert(text.size() < TEXT_QUAD_BUFF_SIZE);

    // get your data
    auto& data = GetData<Font>();

    // get the font atlas, load if necessary
    std::map<int, FontAtlas>::iterator iter = data.font_atlas_map.find(point_size);
    if(iter != data.font_atlas_map.end())
    {
      text_render_data.atlas = iter->second;
    }
    else
    {
      text_render_data.atlas = LoadFontAtlas(point_size);
    }

    int cur_x = start_x, cur_y = start_y;

    // per letter
    for(int i = 0; i < text.size(); ++i)
    {
      // offset the rest of the characters if the current char is a space
      if(text[i] == ' ')
      {
        cur_x += space_size;
      }

      // get the index of the char
      int idx = GetIndexInBaseChar(text[i]);

      // create a quad to be drawn

      // set the uv values
      text_render_data.text_quads[i].min_u = 0.0f;
      text_render_data.text_quads[i].min_v = 0.9999f;
      text_render_data.text_quads[i].max_u = 0.9999f;
      text_render_data.text_quads[i].max_v = 0.0f;

      // store the data in variables with shorter names
      int width    = text_render_data.atlas.w_h_ox_oy[idx][0],
          height   = text_render_data.atlas.w_h_ox_oy[idx][1],
          offset_x = text_render_data.atlas.w_h_ox_oy[idx][2],
          offset_y = text_render_data.atlas.w_h_ox_oy[idx][3];

      // set min values
      text_render_data.text_quads[i].min_x = cur_x;// + offset_x;
      text_render_data.text_quads[i].min_y = cur_y;// + offset_y;

      // update cur_x (and cur_y in the future)
      cur_x += text_render_data.atlas.w_h_ox_oy[idx][0];

      // set the max values
      text_render_data.text_quads[i].max_x = cur_x         ;// + offset_x;
      text_render_data.text_quads[i].max_y = cur_y + height;// + offset_y;

      // set the glyph's texture data
      text_render_data.text_quads[i].glyph_alpha_values = text_render_data.atlas.alpha_values[idx];
      text_render_data.text_quads[i].glyph_width = width;
      text_render_data.text_quads[i].glyph_height = height;

      // ensure a spacing of 16 pixels between the letters for testing purposes
      cur_x += 16;
    }

    // update the num_quads value
    text_render_data.num_quads = text.size();

    // return happily
    return text_render_data;
  }

private:
  friend bool Load(StringRef filename, Data& data)
  {
    (new(&data.font_atlas_map) std::map<int, FontAtlas>());

    if(FileIO::LoadFileBinary(filename.data(), &data.font_file) == false)
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
    auto& data = GetData<Font>();

    // the return value
    FontAtlas font_atlas;

    // four arrays to hold the bitmap sizes per glyph
    int w_h_ox_oy[NUM_BASE_CHARS][4];
    unsigned char* mono_bitmaps[NUM_BASE_CHARS];

    // get the scale that stb_truetype can use
    float scale = stbtt_ScaleForPixelHeight(&data.stb_font, (float)point_size);

    // keep track of the amount of pixels
    size_t tot_size = 0;

    // use stb_truetype to rasterize all of the glyphs for me
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      mono_bitmaps[i] = stbtt_GetCodepointBitmap(&data.stb_font, 0.0f, scale, BASE_CHAR[i],
        &w_h_ox_oy[i][0], &w_h_ox_oy[i][1], &w_h_ox_oy[i][2], &w_h_ox_oy[i][3]);

      tot_size += w_h_ox_oy[i][0] * w_h_ox_oy[i][1];
    }

    // allocate memory for the font textures
    font_atlas.tot_alpha_values = tot_size;
    font_atlas.alpha_values[0] = new unsigned char[tot_size * sizeof(unsigned char)];

    /////////////////////////////////////////////////////////////
    // align all textures underneath each other. 
    /////////////////////////////////////////////////////////////

    // get the initial alpha value
    unsigned char* p = font_atlas.alpha_values[0];

    // for all glyphs we'll use
    for(int i = 0; i < NUM_BASE_CHARS; ++i)
    {
      // reset idx counter
      size_t idx_glyph_tex = 0;

      // set the appropriate font atlas values
      font_atlas.tot_alpha_values = w_h_ox_oy[i][0] * w_h_ox_oy[i][1];
      font_atlas.alpha_values[i] = p;
      font_atlas.w_h_ox_oy[i][0] = w_h_ox_oy[i][0];
      font_atlas.w_h_ox_oy[i][1] = w_h_ox_oy[i][1];
      font_atlas.w_h_ox_oy[i][2] = w_h_ox_oy[i][2];
      font_atlas.w_h_ox_oy[i][3] = w_h_ox_oy[i][3];

      // copy over the alpha values
      memcpy(p, mono_bitmaps[i], font_atlas.tot_alpha_values * sizeof(unsigned char));

      // offset the pointer to the next glyph
      p += font_atlas.tot_alpha_values;
    }

    // place the font atlas in the map
    data.font_atlas_map[point_size] = font_atlas;

    // return the filled out font atlas
    return font_atlas;
  }


  // get the char in the index
  /*
  TODO (Floris) optimize this, waste of performance.
  sort, binary search?
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