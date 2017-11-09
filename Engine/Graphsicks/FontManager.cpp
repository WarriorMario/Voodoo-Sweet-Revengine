#include "FontManager.h"

FontManager::FontManager()
{
  FT_Init_FreeType(&ft_lib);
}

FontManager::~FontManager()
{
  FT_Done_FreeType(ft_lib);
}
