#include "Shader.h"

Shader::Shader(Graphics& gfx)
  :
  gfx(gfx)
{

}

void Shader::Shade(int x, int y)
{
  gfx.PutPixel(x, y, Colors::White);
}
