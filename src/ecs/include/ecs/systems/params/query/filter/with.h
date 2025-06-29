#pragma once

#include "ecs/components/component_concept.h"
#include "ecs/systems/params/query/query_filter.h"

template <typename... T>
    requires IsComponentSetConcept<T...>
class With : public QueryFilter
{
  public:
    using ComponentSetType = std::tuple<T...>;

  public:
    With()
    {
    }

  public:
    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        ComponentIdSet filtered = ComponentTypeRegistry::GetComponentIdSet<T...>();
        return componentIdSet.Include( filtered );
    }
};