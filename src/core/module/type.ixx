export module core.type;

import std;
import icu;

export using i8 = std::int8_t;
export using i16 = std::int16_t;
export using i32 = std::int32_t;
export using i64 = std::int64_t;
export using u8 = std::uint8_t;
export using u16 = std::uint16_t;
export using u32 = std::uint32_t;
export using u64 = std::uint64_t;

export using f32 = float;
export using f64 = double;
#ifdef __clang__
export using usize = std::size_t;
#else
export using usize = size_t;
#endif

export using FsPath = std::filesystem::path;

#pragma region NONE_INDEX

export template <std::integral TIntegral>
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
