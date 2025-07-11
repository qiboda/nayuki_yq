module;

// #include "core/math/math.h"
import core.math;
#include <render_core/render_core.h>

export module render_core.vkconv;

namespace vkconv
{
#pragma region From

template <typename T>
auto From( const T& extent ) -> decltype( auto );

export template <>
inline auto From( const Extent2D& extent ) -> decltype( auto )
{
    return vk::Extent2D( extent.width, extent.height );
}

#pragma endregion // From

#pragma region To

template <typename T>
auto To( const T& extent ) -> decltype( auto );

export template <>
inline auto To( const vk::Extent2D& extent ) -> decltype( auto )
{
    return Extent2D( extent.width, extent.height );
}

#pragma endregion // To

} // namespace vkconv