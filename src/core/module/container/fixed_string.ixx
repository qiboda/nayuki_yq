export module core.container.fixed_string;

import std;
import core;

// trivially copyable 类型
export template <usize N>
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