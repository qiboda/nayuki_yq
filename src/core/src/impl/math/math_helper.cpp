// ***************************************************************************************
// MathHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
// ***************************************************************************************
module;

#include <cmath>

module core.math;

import std;
import core;
import glm;

const f32 MathHelper::Pi = 3.1415926535f;

f32 MathHelper::AngleFromXY( f32 x, f32 y )
{
    f32 theta = 0.0f;

    // Quadrant I or IV
    if ( x >= 0.0f )
    {
        // If x = 0, then atanf(y/x) = +pi/2 if y > 0
        // atanf(y/x) = -pi/2 if y < 0
        theta = atanf( y / x ); // in [-pi/2, +pi/2]

        if ( theta < 0.0f )
        {
            theta += 2.0f * Pi; // in [0, 2*pi).
        }
    }
    // Quadrant II or III
    else
    {
        theta = atanf( y / x ) + Pi; // in [0, 2*pi).
    }
    return theta;
}

Vec3 MathHelper::RandUnitVec3()
{

    // Keep trying until we get a point on/in the hemisphere.
    while ( true )
    {
        // Generate random point in the cube [-1,1]^3.
        Vec3 v = Vec3( MathHelper::RandF( -1.0f, 1.0f ),
                       MathHelper::RandF( -1.0f, 1.0f ),
                       MathHelper::RandF( -1.0f, 1.0f ) );

        // Ignore points outside the unit sphere in order to get an even
        // distribution over the unit sphere.  Otherwise points will clump more
        // on the sphere near the corners of the cube.

        if ( v.length() > 1.0 )
        {
            continue;
        }

        // glm::
        return normalize( v );
    }
}

// glm::vec3 MathHelper::RandHemisphereUnitVec3(glm::vec3 n) {
//     glm::vec3 One = glm::vec3Set(1.0f, 1.0f, 1.0f, 1.0f);
//     glm::vec3 Zero = glm::vec3Zero();

//     // Keep trying until we get a point on/in the hemisphere.
//     while (true) {
//         // Generate random point in the cube [-1,1]^3.
//         glm::vec3 v = glm::vec3Set(MathHelper::RandF(-1.0f, 1.0f),
//                                    MathHelper::RandF(-1.0f, 1.0f),
//                                    MathHelper::RandF(-1.0f, 1.0f), 0.0f);

//         // Ignore points outside the unit sphere in order to get an even
//         // distribution over the unit sphere.  Otherwise points will clump
//         more
//         // on the sphere near the corners of the cube.

//         if (glm::vec33Greater(glm::vec33LengthSq(v), One)) {
//             continue;
//         }

//         // Ignore points in the bottom hemisphere.
//         if (glm::vec33Less(glm::vec33Dot(n, v), Zero)) {
//             continue;
//         }

//         return glm::vec33Normalize(v);
//     }
// }