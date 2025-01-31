#pragma once

#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#include <cstdint>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using ui8 = uint8_t;
using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

using f32 = float;
using f64 = double;

#pragma region NONE_INDEX

namespace NONE_INDEX_Detail {
struct NoneIndex {
    template <typename TIntegral,
              typename = std::enable_if_t<std::is_integral_v<TIntegral>>>
    constexpr operator TIntegral() const {
        return static_cast<TIntegral>(-1);
    }

    template <typename TIntegral, typename>
    friend constexpr bool operator==(TIntegral integral, NoneIndex noneIndex);

    template <typename TIntegral, typename>
    friend constexpr bool operator!=(TIntegral integral, NoneIndex noneIndex);
};

template <typename TIntegral,
          typename = std::enable_if_t<std::is_integral_v<TIntegral>>>
constexpr bool operator==(TIntegral integral, NoneIndex noneIndex) {
    return integral == noneIndex.operator TIntegral();
}

template <typename TIntegral,
          typename = std::enable_if_t<std::is_integral_v<TIntegral>>>
constexpr bool operator!=(TIntegral integral, NoneIndex noneIndex) {
    return (integral == noneIndex) == false;
}

static constexpr NoneIndex noneIndex;
} // namespace NONE_INDEX_Detail

#ifndef NONE_INDEX
#define NONE_INDEX NONE_INDEX_Detail::noneIndex
#endif // !NONE_INDEX

#pragma endregion NONE_INDEX

#endif // TYPE_DEF_H