module;

#include "module_export.h"

export module ecs:system_param;

import std;
import core;
import core.concepts.derived;

export class ECS_API SystemParam
{
  public:
    SystemParam();

    // 子类需要实现此方法
    // static Commands From( Registry* registry, State& state );
};

#pragma region SystemParam
/**
 * @brief
 * 此处 typename T::State 要求 State 是 public 的
 */
export template <typename T>
concept IsSystemParamConcept = IsStrictDerivedConcept<SystemParam, T> && requires { typename T::State; };

export template <typename... T>
inline constexpr bool AllSystemParamsValue = ( IsSystemParamConcept<T> && ... );

export template <typename... T>
concept AllSystemParamsConcept = AllSystemParamsValue<T...>;

#pragma endregion // SystemParam

#pragma region SystemParamRef

export template <typename T>
concept IsSystemParamRefConcept = IsSystemParamConcept<std::decay_t<T>> && std::is_reference_v<T>;

export template <typename... T>
inline constexpr bool AllSystemParamRefsValue = ( IsSystemParamRefConcept<T> && ... );

export template <typename... T>
concept AllSystemParamRefsConcept = AllSystemParamRefsValue<T...>;

#pragma endregion // SystemParamRef

export template <IsSystemParamConcept T>
struct SystemParamTrait
{
};

/**
 * @brief State的存在，允许SystemParam的状态跨帧存在。
 *
 */
export class ECS_API SystemParamState
{
};

export template <IsSystemParamConcept T>
using SystemParamStateType = T::State;

export template <IsSystemParamConcept... T>
using AllSystemParamStateTypes = std::tuple<SystemParamStateType<T>...>;

export template <IsSystemParamConcept T>
constexpr inline bool HasComponentsReadWrite = requires { SystemParamTrait<T>::ComponentsReadWrite; };
