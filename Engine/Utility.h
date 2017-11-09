#pragma once
#include "ChiliMath.h"
#include <cassert>

#ifndef INFINITY
#define INFINITY FLT_MAX
#endif

#define MATH_INLINE _forceinline

// a few global utility functions
template <typename T>
inline void Swap(T& l, T& r)
{
	T temp = l;
	l = r;
	r = temp;
}
template <typename T>
inline T Clamp(T val, T low, T high)
{
	T res = val;
	res = res > high ? high : res;
	res = res < low ? low : res;
	return res;
}
template <typename T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}
template <typename T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}
