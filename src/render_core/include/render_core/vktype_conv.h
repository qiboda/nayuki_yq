#pragma once

#include "core/math/math.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

namespace vkconv
{
#pragma region From

template <typename T>
inline auto From( const T &extent ) -> decltype( auto );

template <>
inline auto From( const Extent2D &extent ) -> decltype( auto )
{
    return vk::Extent2D( extent.width, extent.height );
}

#pragma endregion // From

#pragma region To

template <typename T>
inline auto To( const T &extent ) -> decltype( auto );

template <>
inline auto To( const vk::Extent2D &extent ) -> decltype( auto )
{
    return Extent2D( extent.width, extent.height );
}

#pragma endregion // To

} // namespace vkconv