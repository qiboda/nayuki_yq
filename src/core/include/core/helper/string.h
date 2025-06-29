#pragma once

#include "core/container/small_vector.h"

namespace StringHelper
{

template <usize N>
inline SmallVector<std::string, N> Split( const std::string& s, const std::string& delimiter )
{
    SmallVector<std::string, N> parts;
    usize last_pos = 0;
    while ( true )
    {
        usize pos = s.find_first_of( delimiter );
        if ( pos == std::string::npos )
        {
            if ( last_pos < s.size() )
            {
                parts.push_back( s.substr( last_pos ) );
            }
            break;
        }
        parts.push_back( s.substr( last_pos, pos - last_pos ) );
    }
    return parts;
}

} // namespace StringHelper