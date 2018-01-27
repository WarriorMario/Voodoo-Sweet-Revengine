#include "DrawUtils.h"
#include "Utility\FrameAllocator.h"

void RenderLinePerLine(Renderer& renderer, StringRef text, Font& font,
	int point_size, int start_x, int start_y, const Color& color)
{
	TextRenderData line = font.RenderLine(text, point_size, start_x, start_y);

	unsigned char* alpha_values =
		FrameAllocator::Get().Allocate<unsigned char>(line.tot_height * line.tot_width);

	// render to a single buffer
	for (int i = 0; i < line.num_quads; ++i)
	{
		TextRenderData::GlyphQuad& cur_quad = line.text_quads[i];

		int glyph_alpha_idx = 0;

		int glyph_start_y = cur_quad.min_y - line.start_y;
		int glyph_end_y = glyph_start_y + cur_quad.glyph_height;

		int glyph_start_x = cur_quad.min_x - line.start_x;
		int glyph_end_x = glyph_start_x + cur_quad.glyph_width;

		for (int y = glyph_start_y; y < glyph_end_y; ++y)
		{
			for (int x = glyph_start_x; x < glyph_end_x; ++x, ++glyph_alpha_idx)
			{
				alpha_values[y * line.tot_width + x] = cur_quad.glyph_alpha_values[glyph_alpha_idx];
			}
		}
	}

	float
		min_x = line.text_quads[0].min_x,
		min_y = line.text_quads[0].min_y,
		max_x = line.text_quads[0].min_x + line.tot_width,
		max_y = line.text_quads[0].min_y + line.tot_height;

	float
		min_v = 0.0001f,
		min_u = 0.0001f,
		max_u = 0.9999f,
		max_v = 0.9999f;

	// first triangle   x      y      u      v
	UIShader::PrimData vertices[3];
	vertices[0] = { min_x, min_y, min_u, min_v };
	vertices[1] = { max_x, min_y, max_u, min_v };
	vertices[2] = { min_x, max_y, min_u, max_v };

	UIShader shader = UIShader(
		vertices[0], vertices[1], vertices[2],
		alpha_values,
		line.tot_width,
		line.tot_height,
		color);

	renderer.AddDrawCommand(shader);

	// second triangle
	vertices[0] = { max_x, min_y, max_u, min_v };
	vertices[1] = { max_x, max_y, max_u, max_v };
	vertices[2] = { min_x, max_y, min_u, max_v };

	shader = UIShader(
		vertices[0], vertices[1], vertices[2],
		alpha_values,
		line.tot_width,
		line.tot_height,
		color);

	renderer.AddDrawCommand(shader);
}

void RenderLinePerGlyph(Renderer& renderer, StringRef text, Font& font,
	int point_size, int start_x, int start_y, const Color& color)
{
	TextRenderData line = font.RenderLine(text, point_size, start_x, start_y);

	// create two triangles to be rendered per glyph
	for (int i = 0; i < line.num_quads; ++i)
	{
		TextRenderData::GlyphQuad& cur_quad = line.text_quads[i];

		int glyph_alpha_idx = 0;

		float min_x = cur_quad.min_x;
		float min_y = cur_quad.min_y;
		float max_x = min_x + cur_quad.glyph_width;
		float max_y = min_y + cur_quad.glyph_height;

		float
			min_v = 0.0001f,
			min_u = 0.0001f,
			max_u = 0.9999f,
			max_v = 0.9999f;

		// first triangle   x      y      u      v
		UIShader::PrimData vertices[3];
		vertices[0] = { min_x, min_y, min_u, min_v };
		vertices[1] = { max_x, min_y, max_u, min_v };
		vertices[2] = { min_x, max_y, min_u, max_v };

		UIShader shader = UIShader(
			vertices[0], vertices[1], vertices[2],
			cur_quad.glyph_alpha_values,
			cur_quad.glyph_width,
			cur_quad.glyph_height,
			color);

		renderer.AddDrawCommand(shader);

		// second triangle
		vertices[0] = { max_x, min_y, max_u, min_v };
		vertices[1] = { max_x, max_y, max_u, max_v };
		vertices[2] = { min_x, max_y, min_u, max_v };

		shader = UIShader(
			vertices[0], vertices[1], vertices[2],
			cur_quad.glyph_alpha_values,
			cur_quad.glyph_width,
			cur_quad.glyph_height,
			color);

		renderer.AddDrawCommand(shader);
	}
}

void RenderText(Renderer & renderer, StringRef text, Font & font, int point_size, const b2Vec2 & pos, const Color & color)
{
	RenderLinePerGlyph(renderer, text, font, point_size, pos.x, pos.y, color);
}
