module;

export module ecs:system_param_query_filter_with;

import std;
import :component_info;
import :component_concepts;
import :system_param_query_filter;

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