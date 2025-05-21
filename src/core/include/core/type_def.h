#pragma once

#include <concepts>
#include <cstdint>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;
using usize = size_t;

#pragma region NONE_INDEX

template <std::integral TIntegral>
struct NoneIndex
{
    constexpr operator TIntegral() const
    {
        return static_cast<TIntegral>( -1 );
    }

    friend constexpr bool operator==( TIntegral integral, NoneIndex<TIntegral> noneIndex )
    {
        return integral == noneIndex.operator TIntegral();
    }

    friend constexpr bool operator!=( TIntegral integral, NoneIndex<TIntegral> noneIndex )
    {
        return ( integral == noneIndex ) == false;
    }
};

#pragma endregion NONE_INDEX
