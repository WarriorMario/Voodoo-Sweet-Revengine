#pragma once
#include "FreeType\ft2build.h"
#include FT_FREETYPE_H

/*
  issue: unresolved external symbol with the freetype functions (all of them)

  program can find the .lib file? - nope, or it wouldn't compile when I don't call the functions
  no implementation of the functions in the libs? nope, found them in the c files that i build it from
  wrong run time library? yep, but I fixed that
*/

class FontManager
{
  FontManager()
  {
	  FT_Init_FreeType(&ft_lib);
  }

  ~FontManager()
  {
	  FT_Done_FreeType(ft_lib);
  }

private:
  FT_Library ft_lib;
};

