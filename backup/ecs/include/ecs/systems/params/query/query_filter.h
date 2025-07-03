#pragma once

#include "ecs/components/component_info.h"
#include "ecs/systems/params/query/query_data.h"

class ECS_API QueryFilter
{
  public:
    QueryFilter();

    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        UNUSED_VARS( componentIdSet );
        return true;
    }
};

template <typename T>
constexpr inline bool IsQueryFilterValue = IsStrictDerivedValue<QueryFilter, T>;

template <typename T>
concept IsQueryFilterConcept = IsQueryFilterValue<T>;

template <typename... T>
constexpr inline bool AllQueryFiltersValue = ( IsQueryFilterValue<T> && ... );

template <typename... T>
concept AllQueryFiltersConcept = AllQueryFiltersValue<T...>;

template <typename... T>
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

template <typename T>
constexpr inline bool IsQueryFilterSetValue = IsTypeInstanceOfValue<T, QueryFilterSet>;

template <typename T>
concept IsQueryFilterSetConcept = IsQueryFilterSetValue<T>;