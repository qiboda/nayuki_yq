// ***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
// ***************************************************************************************

#pragma once

#include "module_export.h"
#include <core/type_def.h>
#include <glm/glm.hpp>

class CORE_API MathHelper
{
  public:
    // Returns random f32 in [0, 1).
    static f32 RandF()
    {
        return static_cast<f32>( rand() ) / static_cast<f32>( RAND_MAX );
    }

    // Returns random f32 in [a, b).
    static f32 RandF( f32 a, f32 b )
    {
        return a + RandF() * ( b - a );
    }

    static i32 Rand( i32 a, i32 b )
    {
        return a + rand() % ( ( b - a ) + 1 );
    }

    template <typename T>
    static T Min( const T &a, const T &b )
    {
        return a < b ? a : b;
    }

    template <typename T>
    static T Max( const T &a, const T &b )
    {
        return a > b ? a : b;
    }

    template <typename T>
    static T Lerp( const T &a, const T &b, f32 t )
    {
        return a + ( b - a ) * t;
    }

    template <typename T>
    static T Clamp( const T &x, const T &low, const T &high )
    {
        return x < low ? low : ( x > high ? high : x );
    }

    // Returns the polar angle of the point (x,y) in [0, 2*PI).
    static f32 AngleFromXY( f32 x, f32 y );

    // static glm::vec3 SphericalToCartesian(f32 radius, f32 theta,
    //                                               f32 phi) {
    //     return glm::vec3Set(radius * sinf(phi) * cosf(theta),
    //                                 radius * cosf(phi),
    //                                 radius * sinf(phi) * sinf(theta), 1.0f);
    // }

    static glm::vec3 RandUnitVec3();
    // static glm::vec3 RandHemisphereUnitVec3(glm::vec3 n);

    static const f32 Infinity;
    static const f32 Pi;
};
