#pragma once

#include "LiquidFun/Common/b2Math.h"

// ****************************************************************************
using Mat2 = b2Mat22;
using Mat3 = b2Mat33;
using Vec2 = b2Vec2;
using Vec3 = b2Vec3;
using Rot = b2Rot;
using Transform = b2Transform;

// ****************************************************************************

// Mat2
inline Mat3 operator*(const Mat3& lhs, const Mat3& rhs)
{
  return b2Mul(lhs, rhs);
}

// Mat2
inline Mat2 operator*(const Mat2& lhs, const Mat2& rhs)
{
  return b2Mul(lhs, rhs);
}
inline Mat2 operator*(const Mat2& m, const Rot& rot)
{
  Mat2 r(rot.GetXAxis(), rot.GetYAxis());
  return m * r;
}
inline Mat2 operator*(const Rot& rot, const Mat2& m)
{
  Mat2 r(rot.GetXAxis(), rot.GetYAxis());
  return r * m;
}

// Vec2
inline Vec2 operator*(const Mat2& m, const Vec2& vec)
{
  return b2Mul(m, vec);
}
inline Vec2 operator*(const Vec2& vec, const Mat2& m)
{
  return b2MulT(m, vec);
}
inline Vec2 operator*(const Transform& t, const Vec2& vec)
{
  return b2Mul(t, vec);
}
inline Vec2 operator*(const Vec2& vec, const Transform& t)
{
  return b2MulT(t, vec);
}
inline Vec2 operator*(const Rot& rot, const Vec2& vec)
{
  return b2Mul(rot, vec);
}
inline Vec2 operator*(const Vec2& vec, const Rot& rot)
{
  return b2MulT(rot, vec);
}

// Vec3
inline Vec3 operator*(const Mat3& m, const Vec3& vec)
{
  return b2Mul(m, vec);
}
//inline Vec3 operator*(const Vec3& vec, const Mat3& m)
//{
//  return b2MulT(m, vec);
//}

// Rot
inline Rot operator*(const Rot& lhs, const Rot& rhs)
{
  return b2Mul(lhs, rhs);
}

// Transform
inline Transform operator*(const Transform& lhs, const Transform& rhs)
{
  return b2Mul(lhs, rhs);
}