module;

export module ecs.systems.params.query.filter.without;

import ecs.components.concepts;
import ecs.components.info;
import ecs.systems.params.query.filter;

export template <typename... T>
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