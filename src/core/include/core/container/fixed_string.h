#include "core/type_def.h"
#include <cstring>

// trivially copyable 类型
template <usize N>
struct FixedString
{
    char data[N]{};

    FixedString() = default;

    FixedString( const char* str )
    {
        std::strncpy( data, str, N - 1 );
    }

    bool operator==( const FixedString<N>& other ) const
    {
        return std::strncmp( data, other.data, N ) == 0;
    }
};