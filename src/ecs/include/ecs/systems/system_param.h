#pragma once

#include "module_export.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API SystemParam
{
  public:
    SystemParam();
};

/**
 * @brief
 * 此处 typename T::State 要求 State 是 public 的
 */
template <typename T>
concept IsSystemParamConcept = IsStrictDerivedConcept<SystemParam, T> && requires { typename T::State; };

template <typename... T>
inline constexpr bool AllSystemParams = ( IsSystemParamConcept<T> && ... );

template <typename... T>
concept AllSystemParamsConcept = AllSystemParams<T...>;

template <typename T>
struct SystemParamTrait
{
};

class ECS_API SystemParamState
{
};

template <IsSystemParamConcept T>
using SystemParamStateType = T::State;

template <IsSystemParamConcept... T>
using AllSystemParamStateTypes = std::tuple<SystemParamStateType<T>...>;