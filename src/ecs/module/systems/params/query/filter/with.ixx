module;

export module ecs.systems.params.query.filter.with;

import std;
import ecs.components.info;
import ecs.components.concepts;
import ecs.systems.params.query.filter;

export template <typename... T>
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