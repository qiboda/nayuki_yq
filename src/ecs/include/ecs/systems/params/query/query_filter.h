#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API QueryFilter
{
  public:
    QueryFilter();
};

template <typename T>
constexpr inline bool IsQueryFilterValue = IsStrictDerivedValue<QueryFilter, T>;

template <typename T>
concept IsQueryFilterConcept = IsQueryFilterValue<T>;

template <typename... T>
constexpr inline bool IsQueryFilterSetValue = ( IsQueryFilterValue<T> && ... );

template <typename... T>
concept IsQueryFilterSetConcept = IsQueryFilterSetValue<T...>;

class ECS_API QueryFilterSet
{
  public:
    QueryFilterSet()
    {
    }
};
