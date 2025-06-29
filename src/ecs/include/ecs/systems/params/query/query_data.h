#pragma once

#include "core/concepts/are_unique.h"
#include "ecs/components/component_info.h"
#include "core/concepts/type_instance.h"
#include "ecs/entity/entity.h"

#include <type_traits>

// 或者是Entity，或者是Component的引用，或者是const Component的引用
// 最多允许一个Entity，但是允许多个 Component, 同时要求Component的类型不相同。
// TODO: 暂时先不支持查询参数包

template <typename T>
constexpr inline bool IsQueryData = IsStrictDerivedValue<Component, T>;

template <typename T>
concept IsQueryDataConcept = IsQueryData<T>;

/**
 * @brief 允许有限定符
 */
template <typename T>
concept IsQueryDataQualifiedConcept = IsQueryData<std::decay_t<T>>;

/**
 * @brief 引用类型下is_const_v始终是false
 *
 * @tparam T
 */
template <IsQueryDataQualifiedConcept T>
inline constexpr bool IsWritableQueryData = ( std::is_const_v<std::remove_reference_t<T>> == false );

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

template <typename... T>
concept AllQueryDataArgConcept = ( IsQueryDataArgValue<T> && ... );

template <typename... T>
concept QueryDataSetParamsConcept = AreUniqueValue<T...> && AllQueryDataArgConcept<T...>;

template <typename... T>
    requires QueryDataSetParamsConcept<T...>
class QueryDataSet
{
  public:
    using QueryDataSetParamTypes = std::tuple<T...>;
    using QueryDataSetDecayedParamTypes = std::tuple<std::decay_t<T>...>;

  public:
    constexpr static ComponentIdSet GetComponentIdSet()
    {
        return ComponentTypeRegistry::GetComponentIdSet<std::decay_t<T>...>();
    }
};

template <typename T>
constexpr inline bool IsQueryDataSetValue = IsTypeInstanceOfValue<T, QueryDataSet>;

template <typename T>
concept IsQueryDataSetConcept = IsQueryDataSetValue<T>;