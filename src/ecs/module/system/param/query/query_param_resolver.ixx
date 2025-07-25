module;

#include "module_export.h"

export module ecs:system_param_query_param_resolver;

import std;
import core.type;
import core.concepts.are_unique;
import core.concepts.tuple;
import core.concepts.derived;
import :system_param_query_data;
import :system_param_query_filter;
import :component;
import :entity;

export enum class QueryParamType : u8 {
    None,
    Data,
    Filter,
};

export template <typename... T>
constexpr inline bool IsQueryParamsValue =
    AreUnique<T...>::value && ( ( IsQueryDataArgConcept<T> || IsQueryFilterConcept<T> ) && ... );

export template <typename... T>
concept IsQueryParamsConcept = IsQueryParamsValue<T...>;

export template <typename... T>
    requires IsQueryParamsConcept<T...>
struct QueryParamResolver
{
    using ParamTypes = std::tuple<T...>;

    template <typename TDerived>
    using IsStrictDerivedComponent = IsStrictDerived<Component, TDerived>;
    template <typename TDerived>
    using IsStrictDerivedQueryFilter = IsStrictDerived<QueryFilter, TDerived>;

    using QueryDataTypes = TypePackFilterType<IsStrictDerivedComponent, T...>;
    using QueryDataDecayedTypes = TypePackFilterType<IsStrictDerivedComponent, std::decay_t<T>...>;
    using QueryDataSetType = TupleApplyTo<QueryDataTypes, QueryDataSet>;

    using QueryFilterTypes = TypePackFilterType<IsStrictDerivedQueryFilter, T...>;
    using QueryFilterSetType = TupleApplyTo<QueryFilterTypes, QueryFilterSet>;

    template <typename TType>
    using IsNonEntity = IsNot<IsSame<Entity, TType>>;
    using QueryDataNonEntityTypes = TupleFilterType<IsNonEntity, QueryDataTypes>;

    template <QueryParamType lastType>
    static consteval bool IsOrdered()
    {
        return true;
    }

    template <QueryParamType lastType, typename P1, typename... P>
    static consteval bool IsOrdered()
    {
        constexpr bool isQueryData = IsQueryDataArgValue<P1>;
        constexpr bool isQueryFilter = IsQueryFilterValue<P1>;
        if constexpr ( lastType == QueryParamType::Filter )
        {
            static_assert( isQueryData == false, "QueryData type must before QueryFilter" );
        }
        if constexpr ( isQueryData == true )
        {
            static_assert( isQueryFilter == false,
                           "Query Param can not be QueryData and QueryFilter at the same time" );
        }
        else
        {
            static_assert( isQueryFilter == true, "Query Param can not neither QueryData nor QueryFilter" );
        }

        if constexpr ( sizeof...( P ) == 0 )
        {
            return true;
        }

        if constexpr ( isQueryData )
        {
            return IsOrdered<QueryParamType::Data, P...>();
        }
        if constexpr ( isQueryFilter )
        {
            return IsOrdered<QueryParamType::Filter, P...>();
        }
        return false;
    }

    /**
     * @brief 总应该为True，如果为False，应该编译不过。
     */
    static inline constexpr bool IsOrderedValue = IsOrdered<QueryParamType::None, T...>();
};

export template <typename... T>
inline constexpr bool CanQueryParamResolve = QueryParamResolver<T...>::IsOrderedValue;

export template <typename... T>
concept QueryParamResolverConcept = CanQueryParamResolve<T...>;
