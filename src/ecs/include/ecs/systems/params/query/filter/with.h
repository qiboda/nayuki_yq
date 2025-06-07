#pragma once

#include "ecs/components/component_concept.h"
#include "ecs/systems/params/query/query_filter.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename... T>
    requires IsComponentSetConcept<T...>
class With : public QueryFilter
{
  public:
    With()
    {
    }
};