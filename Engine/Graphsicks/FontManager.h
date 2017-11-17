#pragma once
#include "VArray.h"
#include "Graphsicks\Shader.h"
#include "Graphsicks\Renderer.h"

class FontManager
{
public:
  static void FontManager::RenderLine(TextRenderData& line,
    Renderer& renderer, const Color& color = Colors::White);
};

