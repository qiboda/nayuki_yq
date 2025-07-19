module;

export module ecs:system_param_query_filter_or;

import std;
import core.type;
import :system_param_query_filter;
import :component_info;

export template <typename... T>
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