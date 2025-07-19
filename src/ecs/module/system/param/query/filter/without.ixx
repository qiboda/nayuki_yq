module;

export module ecs:system_param_query_filter_without;

import :component_concepts;
import :component_info;
import :system_param_query_filter;

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