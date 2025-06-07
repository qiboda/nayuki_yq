#pragma once

#include "core/macro/macro.h"
#include "ecs/archetype/archetype.h"
#include "ecs/components/component_info.h"
#include "ecs/entity/entity.h"
#include "ecs/systems/params/query/query_data.h"
#include "ecs/systems/params/query/query_filter.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <unordered_map>
#include <utility>

class ECS_API ArchetypeManager
{
  public:
    ArchetypeManager();

  public:
    // TODO: 需要一个同时添加entity和components的函数
    void CreateEntity( Entity entity )
    {
        // 发现空组件的archetype。或者创建一个新的。
        auto it = mComponentIdsArchetypeMap.find( ComponentTypeRegistry::GetComponentIdSet<Entity>() );
        if ( it != mComponentIdsArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype& archetype = mArchetypes[archetypeIndex];
            archetype.AddEntity( entity );

            mEntityArchetypeMap.emplace( entity, archetypeIndex );
        }
        else
        {
            usize archetypeIndex = mArchetypes.size();

            Archetype archetype;
            archetype.Init( ComponentTypeRegistry::GetComponentIdSet<Entity>() );
            archetype.AddEntity( entity );
            mArchetypes.push_back( archetype );
            mEntityArchetypeMap.emplace( entity, archetypeIndex );

            mComponentIdsArchetypeMap.emplace( ComponentIdSet(), archetypeIndex );
        }
    }

    void DestroyEntity( Entity entity )
    {
        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            it = mEntityArchetypeMap.erase( it );
            usize archetypeIndex = it->second;
            mArchetypes[archetypeIndex].DestroyEntity( entity );
        }
    }

  public:
    // 添加一个全新的component，并且将旧的component数据移动到新的archetype中。
    template <IsComponentConcept... T>
    void AddComponentData( Entity entity, T&&... component )
    {
        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype& archetype = mArchetypes[archetypeIndex];

            ComponentIdSet componentIdSet = archetype.GetComponentIdSet();
            componentIdSet.Merge( ComponentTypeRegistry::GetComponentIdSet<T...>() );

            auto it2 = mComponentIdsArchetypeMap.find( componentIdSet );
            if ( it2 != mComponentIdsArchetypeMap.end() )
            {
                usize newArchetypeIndex = it2->second;
                Archetype& newArchetype = mArchetypes[newArchetypeIndex];

                newArchetype.AddEntityComponents( &archetype, entity, std::forward<T>( component )... );

                mEntityArchetypeMap.insert_or_assign( entity, newArchetypeIndex );
            }
            else
            {
                Archetype newArchetype;
                newArchetype.Init( componentIdSet.Clone() );
                newArchetype.AddEntity( entity );

                newArchetype.AddEntityComponents( &archetype, entity, std::forward<T>( component )... );

                mArchetypes.push_back( newArchetype );
                mEntityArchetypeMap.emplace( entity, archetypeIndex );
                mComponentIdsArchetypeMap.emplace( componentIdSet, archetypeIndex );
            }
        }
    }

    // TODO: 和AddComponentData相似，是否可以合并成一个函数。
    // 移除一个component，并且将旧的component数据移动到新的archetype中。
    template <IsComponentConcept... T>
    void RemoveComponentData( Entity entity, T&&... component )
    {
        ( UNUSED_VAR( component ), ... );

        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype& archetype = mArchetypes[archetypeIndex];

            ComponentIdSet componentIdSet = archetype.GetComponentIdSet();
            componentIdSet.Subtract( ComponentTypeRegistry::GetComponentIdSet<T...>() );

            auto it2 = mComponentIdsArchetypeMap.find( componentIdSet );
            if ( it2 != mComponentIdsArchetypeMap.end() )
            {
                usize newArchetypeIndex = it2->second;
                Archetype& newArchetype = mArchetypes[newArchetypeIndex];
                newArchetype.RemoveEntityComponents( &archetype, entity, component... );

                mEntityArchetypeMap.insert_or_assign( entity, newArchetypeIndex );
            }
            else
            {
                Archetype newArchetype;
                newArchetype.Init( componentIdSet.Clone() );
                newArchetype.AddEntity( entity );

                newArchetype.RemoveEntityComponents( &archetype, entity, component... );

                mArchetypes.push_back( newArchetype );
                mEntityArchetypeMap.emplace( entity, archetypeIndex );
                mComponentIdsArchetypeMap.emplace( componentIdSet, archetypeIndex );
            }
        }
    }

    template <IsComponentConcept... T>
    void ReplaceComponentData( Entity entity, T&&... components )
    {
        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype& archetype = mArchetypes[archetypeIndex];
            archetype.ReplaceEntityComponents( entity, components... );
        }
    }

  public:
    template <typename TQueryDataSet, typename TQueryFilterSet>
        requires IsQueryDataSetConcept<TQueryDataSet> && IsQueryFilterSetConcept<TQueryFilterSet>
    std::vector<usize> FindMetArchetypeIndices() const
    {
        std::vector<usize> met;
        const ComponentIdSet& componentIdSet = TQueryDataSet::GetComponentIdSet();
        for ( usize i = 0; i < mArchetypes.size(); ++i )
        {
            const ComponentIdSet& ArchetypeComponentIdSet = mArchetypes[i].GetComponentIdSet();
            if ( mArchetypes[i].GetTotalEntityNum() > 0 && ArchetypeComponentIdSet.Include( componentIdSet ) &&
                 TQueryFilterSet::Filter( ArchetypeComponentIdSet ) )
            {
                met.push_back( i );
            }
        }

        return met;
    }

    const Archetype* GetArchetype( const usize index ) const
    {
        return &mArchetypes[index];
    }

    Archetype* GetArchetype( const usize index )
    {
        return &mArchetypes[index];
    }

  protected:
    std::vector<Archetype> mArchetypes;
    // value => mArchetype index
    std::unordered_map<Entity, usize> mEntityArchetypeMap;
    // value => mArchetype index
    std::unordered_map<ComponentIdSet, usize> mComponentIdsArchetypeMap;
};
