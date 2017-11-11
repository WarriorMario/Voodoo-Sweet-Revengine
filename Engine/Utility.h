#pragma once
#include "ChiliMath.h"
#include <cassert>
#include <cstring>

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
inline void ZeroMem(void* dest, size_t size)
{
  memset(dest, 0, size);
}
template<typename T, size_t count>
constexpr size_t CountOf(const T(&arr)[count])
{
  return count;
}
template<typename T, size_t count, typename V>
inline bool RangeEquals(const T(&arr)[count], const V& comparison)
{
  bool val = false;
  for(int i = 0; i < count; ++i)
  {
    val |= (arr[i] == comparison);
  }
  return val;
}