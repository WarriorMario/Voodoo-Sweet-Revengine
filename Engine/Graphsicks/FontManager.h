#pragma once
#include "FreeType\ft2build.h"
#include FT_FREETYPE_H


class FontManager
{
  FontManager();
  ~FontManager();

private:
  FT_Library ft_lib;
};