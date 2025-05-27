#pragma once

#include "ecs/systems/query_param.h"
#include "ecs/systems/system_param.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename... T>
    requires IsQueryParamSetConcept<T...>
class Query : public SystemParam
{
  public:
    Query()
        : SystemParam()
    {
    }
};