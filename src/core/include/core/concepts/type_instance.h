#pragma once

#include <type_traits>

template <typename TInstance, template <typename...> class TTemplate>
struct IsTypeInstanceOf : std::false_type
{
};

template <template <typename...> class Template, typename... Args>
struct IsTypeInstanceOf<Template<Args...>, Template> : std::true_type
{
};

template <typename TInstance, template <typename...> class TTemplate>
inline constexpr bool IsTypeInstanceOfValue = IsTypeInstanceOf<TInstance, TTemplate>::value;