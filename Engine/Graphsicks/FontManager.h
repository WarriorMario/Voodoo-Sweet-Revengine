#pragma once
#include "VArray.h"
#include "Graphsicks\Shader.h"
#include "Graphsicks\Renderer.h"

//class FontManager
//{
//private: // should be public, hiding them right now
//	static void StartFrame(Renderer& renderer);
//	static void EndFrame(Renderer& renderer);
//	static void RenderLine(TextRenderData& line,
//		Renderer& renderer, const Color& color = Colors::White);
//
//private:
//	static unsigned char* screen_buffer;
//	static size_t screen_width;
//	static size_t screen_height;
//};

void RenderLinePerLine(StringRef text, int point_size, int start_x, int start_y, 
	Font& font,	Renderer& renderer, const Color& color = Colors::White);

void RenderLinePerGlyph(StringRef text, int point_size, int start_x, int start_y,
	Font& font, Renderer& renderer, const Color& color = Colors::White);
