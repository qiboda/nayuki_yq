#pragma once

#include "ecs/systems/query_param.h"
#include "ecs/systems/system_param.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <utility>

template <typename... T>
    requires IsQueryParamSetConcept<T...>
class Query : public SystemParam
{
  public:
    Query()
        : SystemParam()
    {
    }
};

template <typename... T>
    requires IsQueryParamSetConcept<T...>
struct SystemParamTrait<Query<T...>>
{
    using Type = Query<T...>;

    using QueryParamTypes = std::tuple<T...>;

    static constexpr std::vector<bool> readonly = ParamReadonly( std::make_index_sequence<sizeof...( T )>() );

    template <usize... I>
    consteval std::vector<bool> ParamReadonly( std::index_sequence<I...> )
    {
        std::vector<bool> result( sizeof...( T ) );
        ( ( result[I] = IsReadOnlyQueryParam<T> ), ... );
        return result;
    }
};