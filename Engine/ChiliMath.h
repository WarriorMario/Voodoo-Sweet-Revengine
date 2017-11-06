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

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;
constexpr float epsilon = 1e-4;

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


template<typename T>
inline constexpr T clamp( T val, T low, T high )
{
   return val <= low ? low : val >= high ? high : val;
}

template<typename T>
void fresnel( T cosi, const T &ior, T &kr )
{
   T etai = 1, etat = ior;
   if ( cosi > 0.0f )
   {
      std::swap( etai, etat );
   }
   // Compute sini using Snell's law
   T sint = etai / etat * sqrtf( std::max( 0.f, 1.0f - cosi * cosi ) );
   // Total internal reflection
   if ( sint >= 1.0f )
   {
      kr = 1.0f;
   }
   else
   {
      T cost = sqrtf( std::max( 0.f, 1.0f - sint * sint ) );
      cosi = fabsf( cosi );
      T Rs = ( ( etat * cosi ) - ( etai * cost ) ) / ( ( etat * cosi ) + ( etai * cost ) );
      T Rp = ( ( etai * cosi ) - ( etat * cost ) ) / ( ( etai * cosi ) + ( etat * cost ) );
      kr = ( Rs * Rs + Rp * Rp ) / 2.0f;
   }
   // As a consequence of the conservation of energy, transmittance is given by:
   // kt = 1 - kr;
}
template <typename T>
inline void Swap( T& l, T& r )
{
   T temp = l;
   l = r;
   r = temp;
}
template <typename T>
inline T Clamp( T val, T low, T high )
{
   T res = val;
   res = res > high ? high : res;
   res = res < low ? low : res;
   return res;
}
template <typename T>
inline T min( T a, T b )
{
   return a < b ? a : b;
}
template <typename T>
inline T max( T a, T b )
{
   return a > b ? a : b;
}