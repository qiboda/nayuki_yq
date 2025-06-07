#pragma once

#include "ecs/components/component_concept.h"
#include "ecs/systems/params/query/query_filter.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename... T>
    requires IsComponentSetConcept<T...>
class Without : public QueryFilter
{
  public:
    Without()
    {
    }

  public:
    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        ComponentIdSet filtered = ComponentTypeRegistry::GetComponentIdSet<T...>();
        return componentIdSet.Exclude( filtered );
    }
};