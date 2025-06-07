#pragma once

#include "ecs/entity/entity.h"
#include "core/concepts/are_unique.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <type_traits>

// 或者是Entity，或者是Component的引用，或者是const Component的引用
// 最多允许一个Entity，但是允许多个 Component, 同时要求Component的类型不相同。
// TODO: 暂时先不支持查询参数包
class ECS_API QueryDataSet
{
  public:
    QueryDataSet();
};

template <typename T>
constexpr inline bool IsQueryData = IsStrictDerivedValue<Component, T>;

template <typename T>
concept IsQueryDataConcept = IsQueryData<T>;

template <IsQueryDataConcept T>
inline constexpr bool IsReadOnlyQueryData = std::is_const_v<T>;

template <IsQueryDataConcept T>
inline constexpr bool IsReadWriteQueryData = ( std::is_const_v<T> == false );

template <typename T>
concept EntityArgsLimitConcept =
    !std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;

template <typename T>
concept NonEntityComponentArgsLimitConcept = !std::is_pointer_v<T> && std::is_reference_v<T> && !std::is_volatile_v<T>;

template <typename T>
constexpr inline bool IsQueryDataArgValue =
    // entity 也是component
    IsStrictDerivedValue<Component, std::decay_t<T>> &&
    ( IsEntityValue<std::decay_t<T>> ? EntityArgsLimitConcept<T> : NonEntityComponentArgsLimitConcept<T> );

template <typename T>
concept IsQueryDataArgConcept = IsQueryDataArgValue<T>;
