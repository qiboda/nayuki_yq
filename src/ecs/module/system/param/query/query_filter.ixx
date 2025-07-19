module;

#include "module_export.h"
#include <core/macro/macro.h>

export module ecs:system_param_query_filter;

import std;
import core.type;
import core.concepts.derived;
import core.concepts.type_instance;
import :component_info;
import :system_param_query_data;

export class ECS_API QueryFilter
{
  public:
    QueryFilter();

    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        UNUSED_VARS( componentIdSet );
        return true;
    }
};

export template <typename T>
constexpr inline bool IsQueryFilterValue = IsStrictDerivedValue<QueryFilter, T>;

export template <typename T>
concept IsQueryFilterConcept = IsQueryFilterValue<T>;

export template <typename... T>
constexpr inline bool AllQueryFiltersValue = ( IsQueryFilterValue<T> && ... );

export template <typename... T>
concept AllQueryFiltersConcept = AllQueryFiltersValue<T...>;

export template <typename... T>
    requires AllQueryFiltersConcept<T...>
class QueryFilterSet
{
    using FilterParamTypes = std::tuple<T...>;

  public:
    QueryFilterSet()
    {
    }

    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        return FilterImpl( componentIdSet, std::make_index_sequence<sizeof...( T )>() );
    }

  protected:
    template <usize... I>
    static bool FilterImpl( const ComponentIdSet& componentIdSet, std::index_sequence<I...> )
    {
        return ( std::tuple_element_t<I, T>::Filter( componentIdSet ) && ... );
    }
};

export template <typename T>
constexpr inline bool IsQueryFilterSetValue = IsTypeInstanceOfValue<T, QueryFilterSet>;

export template <typename T>
concept IsQueryFilterSetConcept = IsQueryFilterSetValue<T>;