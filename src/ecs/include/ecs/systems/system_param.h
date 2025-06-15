#pragma once

#include "module_export.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API SystemParam
{
  public:
    SystemParam();
};

#pragma region SystemParam
/**
 * @brief
 * 此处 typename T::State 要求 State 是 public 的
 */
template <typename T>
concept IsSystemParamConcept = IsStrictDerivedConcept<SystemParam, T> && requires { typename T::State; };

template <typename... T>
inline constexpr bool AllSystemParamsValue = ( IsSystemParamConcept<T> && ... );

template <typename... T>
concept AllSystemParamsConcept = AllSystemParamsValue<T...>;

#pragma endregion // SystemParam

#pragma region SystemParamRef

template <typename T>
concept IsSystemParamRefConcept = IsSystemParamConcept<std::decay_t<T>> && std::is_reference_v<T>;

template <typename... T>
inline constexpr bool AllSystemParamRefsValue = ( IsSystemParamRefConcept<T> && ... );

template <typename... T>
concept AllSystemParamRefsConcept = AllSystemParamRefsValue<T...>;

#pragma endregion // SystemParamRef

template <IsSystemParamConcept T>
struct SystemParamTrait
{
};

/**
 * @brief State的存在，允许SystemParam的状态跨帧存在。
 *
 */
class ECS_API SystemParamState
{
};

template <IsSystemParamConcept T>
using SystemParamStateType = T::State;

template <IsSystemParamConcept... T>
using AllSystemParamStateTypes = std::tuple<SystemParamStateType<T>...>;

template <IsSystemParamConcept T>
constexpr inline bool HasComponentsReadWrite = requires { SystemParamTrait<T>::ComponentsReadWrite; };
