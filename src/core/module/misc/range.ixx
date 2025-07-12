export module core.misc.range;

import std;

// 简单的 enumerate 实现，需支持 views::zip
namespace detail
{
export struct enumerate_adapter : std::ranges::range_adaptor_closure<enumerate_adapter>
{
    template <std::ranges::viewable_range R>
    constexpr auto operator()( R&& r ) const
    {
        return std::views::zip( std::views::iota( 0u ), std::forward<R>( r ) );
    }
};
} // namespace detail

export inline constexpr detail::enumerate_adapter enumerate;
