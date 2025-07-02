module;

#include <module_export.h>

export module core.math:helper;

import glm;
import std;
import core;

export class CORE_API MathHelper
{
  public:
    // Returns random f32 in [0, 1).
    static f32 RandF()
    {
        std::random_device rd;
        std::mt19937 gen( rd() );
        std::uniform_real_distribution<float> dist( 0.0f, 1.0f );
        return dist( gen );
    }

    // Returns random f32 in [a, b).
    static f32 RandF( f32 a, f32 b )
    {
        std::random_device rd;
        std::mt19937 gen( rd() );
        std::uniform_real_distribution<float> dist( a, b );
        return dist( gen );
    }

    static i32 Rand( i32 a, i32 b )
    {
        std::random_device rd;
        std::mt19937 gen( rd() );
        std::uniform_int_distribution<i32> dist( a, b );
        return dist( gen );
    }

    template <typename T>
    static T Min( const T& a, const T& b )
    {
        return a < b ? a : b;
    }

    template <typename T>
    static T Max( const T& a, const T& b )
    {
        return a > b ? a : b;
    }

    template <typename T>
    static T Lerp( const T& a, const T& b, f32 t )
    {
        return a + ( b - a ) * t;
    }

    template <typename T>
    static T Clamp( const T& x, const T& low, const T& high )
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

    static Vec3 RandUnitVec3();
    // static vec3 RandHemisphereUnitVec3(glm::vec3 n);

    static const f32 Infinity;
    static const f32 Pi;
};
