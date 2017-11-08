#pragma once
#include "Colors.h"
#include "Graphics.h"

class Shader
{
public:
  Shader(Graphics& gfx);
  void Shade(int x, int y);

public:
  Graphics& gfx;
};
