export module core.concepts.are_unique;

import std;

export template <typename... T>
struct Contains : std::false_type
{
};

export template <typename T, typename U>
struct Contains<T, U>
    : std::conditional<std::is_same_v<std::decay_t<T>, std::decay_t<U>>, std::true_type, std::false_type>::type
{
};

export template <typename T, typename U, typename... Types>
struct Contains<T, U, Types...> : std::conditional<std::is_same_v<std::decay_t<T>, std::decay_t<U>>,
                                                   std::true_type,
                                                   Contains<std::decay_t<T>, Types...>>::type
{
};

// Helper to check if all types in a type list are unique
export template <typename... Types>
struct AreUnique : std::false_type
{
};

export template <typename T>
struct AreUnique<T> : std::true_type
{
};

export template <typename T, typename U>
struct AreUnique<T, U> : std::integral_constant<bool, !Contains<T, U>::value>
{
};

export template <typename T, typename... Types>
struct AreUnique<T, Types...>
    : std::integral_constant<bool, ( !Contains<std::decay_t<T>, Types...>::value ) && AreUnique<Types...>::value>
{
};

export template <typename... T>
inline constexpr bool AreUniqueValue = AreUnique<T...>::value;

export template <typename T>
concept AreUniqueConcept = AreUniqueValue<T>;