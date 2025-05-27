#pragma once

#include "ecs/entity/entity.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <type_traits>

// 或者是Entity，或者是Component的引用，或者是const Component的引用
// 最多允许一个Entity，但是允许多个 Component, 同时要求Component的类型不相同。
// TODO: 暂时先不支持查询参数包
class ECS_API QueryParam
{
  public:
    QueryParam();
};

template <typename... T>
struct Contains : std::false_type
{
};

template <typename T, typename U>
struct Contains<T, U>
    : std::conditional<std::is_same_v<std::decay_t<T>, std::decay_t<U>>, std::true_type, std::false_type>::type
{
};

template <typename T, typename U, typename... Types>
struct Contains<T, U, Types...> : std::conditional<std::is_same_v<std::decay_t<T>, std::decay_t<U>>,
                                                   std::true_type,
                                                   Contains<std::decay_t<T>, Types...>>::type
{
};

// Helper to check if all types in a type list are unique
template <typename... Types>
struct AreUnique : std::false_type
{
};

template <typename T>
struct AreUnique<T> : std::true_type
{
};

template <typename T, typename U>
struct AreUnique<T, U> : std::integral_constant<bool, !Contains<T, U>::value>
{
};

template <typename T, typename... Types>
struct AreUnique<T, Types...>
    : std::integral_constant<bool, ( !Contains<std::decay_t<T>, Types...>::value ) && AreUnique<Types...>::value>
{
};

template <typename... T>
inline constexpr bool AreUniqueValue = AreUnique<T...>::value;

template <typename T>
concept EntityParamLimitConcept =
    !std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;

template <typename T>
concept NonEntityComponentParamLimitConcept = !std::is_pointer_v<T> && std::is_reference_v<T> && !std::is_volatile_v<T>;

template <typename... T>
concept IsQueryParamSetConcept =
    AreUnique<T...>::value &&
    // entity 也是componen
    ( std::is_base_of_v<Component, std::decay_t<T>> && ... ) &&
    ( ( IsEntityValue<std::decay_t<T>> ? EntityParamLimitConcept<T> : NonEntityComponentParamLimitConcept<T> ) && ... );