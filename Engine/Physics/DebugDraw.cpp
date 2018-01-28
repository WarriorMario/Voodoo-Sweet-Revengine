#include "DebugDraw.h"
#include "Graphsicks\Graphics.h"
#include "Gameplay\Camera.h"
#include "PhysicsConstants.h"
#include <malloc.h>

void PhysicsDebugDraw::DrawPolygon(const Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	// make a copy of the memory
	Vec2* vertices_copy = (Vec2*)alloca(vertexCount * sizeof(Vec2));
	for (int i = 0; i < vertexCount; ++i)
	{
		vertices_copy[i] = vertices[i];
	}

	// transform vertices with the player camera
	TransformVertices(vertices_copy, vertexCount);

	// Loop vertices
	for (int i = 0; i < vertexCount - 1; ++i)
	{
		gfx.DrawLineClip(PHYSICS_SCALE * vertices_copy[i], PHYSICS_SCALE * vertices_copy[i + 1], Color(color.r * 255, color.g * 255, color.b * 255), RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
	}
	gfx.DrawLineClip(PHYSICS_SCALE * vertices_copy[vertexCount - 1], PHYSICS_SCALE * vertices_copy[0], Color(color.r * 255, color.g * 255, color.b * 255), RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
}

void PhysicsDebugDraw::DrawSolidPolygon(const Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	// transform vertices with the player camera
	TransformVertices(vertices, vertexCount);

	// draw the polygon with the now transformed vertices
	DrawPolygon(vertices, vertexCount, color);
}

void PhysicsDebugDraw::DrawCircle(const Vec2 & center, float32 radius, const b2Color & color)
{
	// Temp we need a faster and colour thing here
	gfx.DrawClippedLineCircle(PHYSICS_SCALE * radius, PHYSICS_SCALE * center, 8, RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
}

void PhysicsDebugDraw::DrawSolidCircle(const Vec2 & center, float32 radius, const Vec2 & axis, const b2Color & color)
{
	DrawCircle(center, radius, color);
}

void PhysicsDebugDraw::DrawParticles(const Vec2 * centers, float32 radius, const b2ParticleColor * colors, int32 count)
{
	for (int i = 0; i < count; ++i)
	{
		// Temp we need a faster and colour thing here
		gfx.DrawClippedLineCircle(PHYSICS_SCALE * radius, PHYSICS_SCALE * centers[i], 8, RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
	}
}

void PhysicsDebugDraw::DrawSegment(const Vec2 & p1, const Vec2 & p2, const b2Color & color)
{
	gfx.DrawLineClip(PHYSICS_SCALE * p1, PHYSICS_SCALE * p2, Color(color.r * 255, color.g * 255, color.b * 255), RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
}

void PhysicsDebugDraw::DrawTransform(const b2Transform & xf)
{
	gfx.DrawClippedLineCircle(5, PHYSICS_SCALE * xf.p, 4, RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
}

void PhysicsDebugDraw::TransformVertices(const Vec2 * vertices, int32 vertexCount)
{
	for (int i = 0; i < vertexCount; ++i)
	{
		vertices[i];
	}
}
