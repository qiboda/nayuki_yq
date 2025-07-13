module;

// #include "core/math/math.h"
#include "module_export.h"
#include <render_core/render_core.h>

export module render_core.vkconv;

import core.math;

namespace vkconv
{
#pragma region From

export template <typename T>
auto From( const T& extent ) -> decltype( auto );

export template <>
inline auto From( const Extent2D& extent ) -> decltype( auto )
{
    return vk::Extent2D( extent.width, extent.height );
}

#pragma endregion // From

#pragma region To

export template <typename T>
auto To( const T& extent ) -> decltype( auto );

export template <>
inline auto To( const vk::Extent2D& extent ) -> decltype( auto )
{
    return Extent2D( extent.width, extent.height );
}

#pragma endregion // To

} // namespace vkconv