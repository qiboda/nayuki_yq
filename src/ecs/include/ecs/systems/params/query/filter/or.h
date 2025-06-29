#pragma once

#include "ecs/systems/params/query/query_filter.h"

template <typename... T>
    requires AllQueryFiltersConcept<T...>
class Or : public QueryFilter
{
  public:
    using ParamFilterTypes = std::tuple<T...>;

  public:
    Or()
    {
    }

  public:
    static bool Filter( const ComponentIdSet& componentIdSet )
    {
        return FilterImpl( componentIdSet, std::make_index_sequence<sizeof...( T )>{} );
    }

  protected:
    template <usize I>
    static bool FilterImpl( const ComponentIdSet& componentIdSet, std::index_sequence<I> )
    {
        return ( std::tuple_element_t<I, T>::Filter( componentIdSet ) && ... );
    }
};