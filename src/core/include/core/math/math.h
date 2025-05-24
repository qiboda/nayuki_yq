#pragma once

#include "core/minimal.h"

struct CORE_API Extent2D
{
    u32 width;
    u32 height;

    Extent2D( u32 w, u32 h )
        : width( w )
        , height( h )
    {
    }

    Extent2D()
        : width( 0 )
        , height( 0 )
    {
    }

    bool operator==( const Extent2D &other ) const
    {
        return width == other.width && height == other.height;
    }

    bool operator!=( const Extent2D &other ) const
    {
        return !( *this == other );
    }
};