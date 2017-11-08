/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	ChiliMath.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <math.h>

// a few global defines
static constexpr float PI = 3.1415926535897932384626433832795;
static constexpr float TWO_PI = 6.283185307179586476925286766559;
static constexpr float HALF_PI = 1.5707963267948966192313216916398;
static constexpr float INV_PI = 0.31830988618379067153776752674503;
static constexpr float INV_TWO_PI = 0.15915494309189533576888376337251;
static constexpr float INV_FOUR_PI = 0.07957747154594766788444188168626;

static constexpr float INV_LOG2 = 3.3219280948873623478703194294894;

constexpr float epsilon = 1e-5;

template <typename T>
inline auto sq( const T& x )
{
   return x * x;
}

template<typename T>
inline T wrap_angle( T theta )
{
   const T modded = fmod( theta, ( T )2.0 * ( T )PI_D );
   return ( modded > ( T )PI_D ) ?
      ( modded - ( T )2.0 * ( T )PI_D ) :
      modded;
}

inline float Deg2Rad(float deg)
{
	return (PI / 180.0f) * deg;
}
inline float Rad2Def(float rad)
{
	return (180.0f / PI) * rad;
}
inline float Log2(float x)
{
	return log(x) * INV_LOG2;
}
inline int Log2int(float x)
{
	return (int)Log2(x);
}
inline bool IsPowerOf2(int v)
{
	return (v & (v - 1)) == 0;
}
inline int RoundUpPow2(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}
inline float Lerp(float v1, float v2, float t)
{
	return (1.0f - t) * v1 + t * v2;
}
