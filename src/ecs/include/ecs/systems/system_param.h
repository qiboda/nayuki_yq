#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include <type_traits>

class ECS_API SystemParam
{
  public:
    SystemParam();
};

template <typename T>
concept IsSystemParamConcept = std::is_base_of_v<SystemParam, T>;

template <typename... T>
concept IsSystemParamSetConcept = ( IsSystemParamConcept<T> && ... );
