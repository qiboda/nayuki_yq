#pragma once

#include "core/macro/macro.h"
#include "ecs/archetype/archetype.h"
#include "ecs/archetype/archetype_chunk.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
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
        auto it = mComponentIdsArchetypeMap.find( ComponentIdSet() );
        if ( it != mComponentIdsArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype &archetype = mArchetypes[archetypeIndex];
            archetype.AddEntity( entity );

            mEntityArchetypeMap.emplace( entity, archetypeIndex );
        }
        else
        {
            usize archetypeIndex = mArchetypes.size();

            Archetype archetype;
            archetype.Init( ComponentIdSet() );
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
    void AddComponentData( Entity entity, T &&...component )
    {
        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype &archetype = mArchetypes[archetypeIndex];
            ComponentIdSet componentIdSet = archetype.mComponentIdSet;

            componentIdSet.Merge( ComponentTypeRegistry::GetComponentIdSet<T...>() );

            auto it2 = mComponentIdsArchetypeMap.find( componentIdSet );
            if ( it2 != mComponentIdsArchetypeMap.end() )
            {
                usize newArchetypeIndex = it2->second;
                Archetype &newArchetype = mArchetypes[newArchetypeIndex];

                ArchetypeChunk *newArchetypeChunk = newArchetype.GetEntityArchetypeChunk( entity );
                NY_ASSERT( newArchetypeChunk != nullptr )

                archetype.MoveEntity( entity, &newArchetype, newArchetypeChunk );
                newArchetype.AddEntityComponents( entity, std::forward<T>( component )... );

                mEntityArchetypeMap.insert_or_assign( entity, newArchetypeIndex );
            }
            else
            {
                Archetype newArchetype;
                newArchetype.Init( componentIdSet.Clone() );
                newArchetype.AddEntity( entity );

                ArchetypeChunk *newArchetypeChunk = newArchetype.GetEntityArchetypeChunk( entity );
                NY_ASSERT( newArchetypeChunk != nullptr )

                archetype.MoveEntity( entity, &newArchetype, newArchetypeChunk );
                newArchetype.AddEntityComponents( entity, std::forward<T>( component )... );

                mArchetypes.push_back( newArchetype );
                mEntityArchetypeMap.emplace( entity, archetypeIndex );
                mComponentIdsArchetypeMap.emplace( componentIdSet, archetypeIndex );
            }
        }
    }

    // TODO: 和AddComponentData相似，是否可以合并成一个函数。
    // 移除一个component，并且将旧的component数据移动到新的archetype中。
    template <IsComponentConcept... T>
    void RemoveComponentData( Entity entity, T &&...component )
    {
        ( UNUSED_VAR( component ), ... );

        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype &archetype = mArchetypes[archetypeIndex];
            ComponentIdSet componentIdSet = archetype.mComponentIdSet;

            componentIdSet.Exclusvie( ComponentTypeRegistry::GetComponentIdSet<T...>() );

            auto it2 = mComponentIdsArchetypeMap.find( componentIdSet );
            if ( it2 != mComponentIdsArchetypeMap.end() )
            {
                usize newArchetypeIndex = it2->second;
                Archetype &newArchetype = mArchetypes[newArchetypeIndex];

                ArchetypeChunk *newArchetypeChunk = newArchetype.GetEntityArchetypeChunk( entity );
                NY_ASSERT( newArchetypeChunk != nullptr )

                archetype.MoveEntity( entity, &newArchetype, newArchetypeChunk );

                mEntityArchetypeMap.insert_or_assign( entity, newArchetypeIndex );
            }
            else
            {
                Archetype newArchetype;
                newArchetype.Init( componentIdSet.Clone() );
                newArchetype.AddEntity( entity );

                ArchetypeChunk *newArchetypeChunk = newArchetype.GetEntityArchetypeChunk( entity );
                NY_ASSERT( newArchetypeChunk != nullptr )

                archetype.MoveEntity( entity, &newArchetype, newArchetypeChunk );

                mArchetypes.push_back( newArchetype );
                mEntityArchetypeMap.emplace( entity, archetypeIndex );
                mComponentIdsArchetypeMap.emplace( componentIdSet, archetypeIndex );
            }
        }
    }

    template <IsComponentConcept... T>
    void ReplaceComponentData( Entity entity, T &&...components )
    {
        auto it = mEntityArchetypeMap.find( entity );
        if ( it != mEntityArchetypeMap.end() )
        {
            usize archetypeIndex = it->second;
            Archetype &archetype = mArchetypes[archetypeIndex];
            archetype.ReplaceEntityComponents( entity, components... );
        }
    }

  protected:
    std::vector<Archetype> mArchetypes;
    // value => mArchetype index
    std::unordered_map<Entity, usize> mEntityArchetypeMap;
    // value => mArchetype index
    std::unordered_map<ComponentIdSet, usize> mComponentIdsArchetypeMap;
};
