export module core.concepts.type_instance;

import std;

export template <typename TInstance, template <typename...> class TTemplate>
struct IsTypeInstanceOf : std::false_type
{
};

export template <template <typename...> class Template, typename... Args>
struct IsTypeInstanceOf<Template<Args...>, Template> : std::true_type
{
};

export template <typename TInstance, template <typename...> class TTemplate>
inline constexpr bool IsTypeInstanceOfValue = IsTypeInstanceOf<TInstance, TTemplate>::value;