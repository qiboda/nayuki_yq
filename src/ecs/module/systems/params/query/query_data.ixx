module;

#include "module_export.h"

export module ecs.systems.params.query.data;

import core.concepts.derived;
import core.concepts.are_unique;
import core.concepts.type_instance;
import ecs.components.info;
import ecs.components.component;
import ecs.entity.entity;
import std;

// 或者是Entity，或者是Component的引用，或者是const Component的引用
// 最多允许一个Entity，但是允许多个 Component, 同时要求Component的类型不相同。
// TODO: 暂时先不支持查询参数包

export template <typename T>
constexpr inline bool IsQueryData = IsStrictDerivedValue<Component, T>;

export template <typename T>
concept IsQueryDataConcept = IsQueryData<T>;

/**
 * @brief 允许有限定符
 */
export template <typename T>
concept IsQueryDataQualifiedConcept = IsQueryData<std::decay_t<T>>;

/**
 * @brief 引用类型下is_const_v始终是false
 *
 * @tparam T
 */
export template <IsQueryDataQualifiedConcept T>
inline constexpr bool IsWritableQueryData = ( std::is_const_v<std::remove_reference_t<T>> == false );

export template <typename T>
concept EntityArgsLimitConcept =
    !std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;

export template <typename T>
concept NonEntityComponentArgsLimitConcept = !std::is_pointer_v<T> && std::is_reference_v<T> && !std::is_volatile_v<T>;

export template <typename T>
constexpr inline bool IsQueryDataArgValue =
    // entity 也是component
    IsStrictDerivedValue<Component, std::decay_t<T>> &&
    ( IsEntityValue<std::decay_t<T>> ? EntityArgsLimitConcept<T> : NonEntityComponentArgsLimitConcept<T> );

export template <typename T>
concept IsQueryDataArgConcept = IsQueryDataArgValue<T>;

export template <typename... T>
concept AllQueryDataArgConcept = ( IsQueryDataArgValue<T> && ... );

export template <typename... T>
concept QueryDataSetParamsConcept = AreUniqueValue<T...> && AllQueryDataArgConcept<T...>;

export template <typename... T>
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

export template <typename T>
constexpr inline bool IsQueryDataSetValue = IsTypeInstanceOfValue<T, QueryDataSet>;

export template <typename T>
concept IsQueryDataSetConcept = IsQueryDataSetValue<T>;