#pragma once
#include "LiquidFun\Common\b2Draw.h"
#include "MatrixDef.h"

class PhysicsDebugDraw : public b2Draw
{
public:
  PhysicsDebugDraw(class Graphics&gfx)
    :
    gfx(gfx)
  {
    SetFlags(e_shapeBit| e_jointBit| e_aabbBit| e_pairBit| e_centerOfMassBit| e_particleBit);
  }
  virtual void DrawPolygon(const Vec2 * vertices, int32 vertexCount, const b2Color & color) override;
  virtual void DrawSolidPolygon(const Vec2 * vertices, int32 vertexCount, const b2Color & color) override;
  virtual void DrawCircle(const Vec2 & center, float32 radius, const b2Color & color) override;
  virtual void DrawSolidCircle(const Vec2 & center, float32 radius, const Vec2 & axis, const b2Color & color) override;
  virtual void DrawParticles(const Vec2 * centers, float32 radius, const b2ParticleColor * colors, int32 count) override;
  virtual void DrawSegment(const Vec2 & p1, const Vec2 & p2, const b2Color & color) override;
  virtual void DrawTransform(const b2Transform & xf) override;
private:
  class Graphics& gfx;
};