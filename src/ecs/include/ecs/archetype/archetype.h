#pragma once

#include "ecs/archetype/archetype_chunk.h"
#include "ecs/archetype/define.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include "module_export.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <utility>
#include <vector>

// 组件id组合
class ECS_API Archetype
{
  public:
    Archetype();

  public:
    inline void AddEntity( Entity entity )
    {
        if ( mArchetypeChunks.empty() )
        {
            AddNewArchetypeChunk();
        }

        bool isAdded = false;
        for ( auto &archetypeChunk : mArchetypeChunks )
        {
            NY_ASSERT( archetypeChunk.ContainEntity( entity ) == false )
            if ( archetypeChunk.CanAddEntity() )
            {
                archetypeChunk.AddEntity( entity );
                isAdded = true;
                break;
            }
        }

        if ( isAdded == false )
        {
            AddNewArchetypeChunk();
            mArchetypeChunks.back().AddEntity( entity );
        }
    }

  protected:
    void AddNewArchetypeChunk()
    {
        mArchetypeChunks.emplace_back( MaxEntityNum, GetArchetypeChunkDataSize() );
    }

  public:
    inline void DestroyEntity( Entity entity )
    {
        ArchetypeChunk *archetypeChunk = GetEntityArchetypeChunk( entity );
        if ( archetypeChunk )
        {
            archetypeChunk->DestroyEntity( entity, mComponentMemoryInfo );
        }
    }

    void MoveEntity( Entity entity, Archetype *destArchetype, ArchetypeChunk *destArchetypeChunk );

    ArchetypeChunk *GetEntityArchetypeChunk( Entity entity )
    {
        for ( auto &archetypeChunk : mArchetypeChunks )
        {
            if ( archetypeChunk.ContainEntity( entity ) )
            {
                return &archetypeChunk;
            }
        }
        return nullptr;
    }

  public:
    template <IsComponentConcept... T>
    void AddEntityComponents( Entity entity, const T &&...components )
    {
        AddEntityComponentsWithIndex( entity,
                                      std::make_index_sequence<sizeof...( T )>{},
                                      std::forward<T>( components )... );
    }

    template <IsComponentConcept... T, usize... I>
    void AddEntityComponentsWithIndex( Entity entity, std::index_sequence<I...> index, const T &&...components )
    {
        ArchetypeChunk *curArchetypeChunk = GetEntityArchetypeChunk( entity );
        if ( curArchetypeChunk )
        {
            ( curArchetypeChunk->AddComponentData( entity,
                                                   GetComponentMemoryInfo( index ),
                                                   std::forward<T>( components ) ),
              ... );
        }
    }

  public:
    template <IsComponentConcept... T>
    void ReplaceEntityComponents( Entity entity, const T &&...components )
    {
        ReplaceEntityComponentsWithIndex( entity,
                                          std::make_index_sequence<sizeof...( T )>{},
                                          std::forward<T>( components )... );
    }

    template <IsComponentConcept... T, usize... I>
    void ReplaceEntityComponentsWithIndex( Entity entity, std::index_sequence<I...> index, const T &&...components )
    {
        ArchetypeChunk *curArchetypeChunk = GetEntityArchetypeChunk( entity );
        if ( curArchetypeChunk )
        {
            ( curArchetypeChunk->ReplaceComponentData( entity,
                                                       GetComponentMemoryInfo( index ),
                                                       std::forward<T>( components ) ),
              ... );
        }
    }

  public:
    void Init( ComponentIdSet &&componentIdSet )
    {
        mComponentIdSet = std::move( componentIdSet );
        ComputeComponentMemoryInfo();
    }

    void ComputeComponentMemoryInfo()
    {
        usize cumulativeSize = 0;
        for ( usize i = 0; i < mComponentIdSet.Get().size(); ++i )
        {
            auto componentInfo = ComponentTypeRegistry::GetComponentInfo( mComponentIdSet.Get()[i] );
            cumulativeSize += componentInfo.size;
        }

        usize totalSize = GetArchetypeChunkDataSize();
        MaxEntityNum = totalSize / cumulativeSize;

        mComponentMemoryInfo.resize( mComponentIdSet.Get().size() );
        for ( usize i = 0; i < mComponentIdSet.Get().size(); ++i )
        {
            auto componentInfo = ComponentTypeRegistry::GetComponentInfo( mComponentIdSet.Get()[i] );
            if ( i == 0 )
            {
                mComponentMemoryInfo[i].mTotalOffset = 0;
            }
            else
            {
                mComponentMemoryInfo[i].mTotalOffset =
                    mComponentMemoryInfo[i - 1].mTotalOffset + mComponentMemoryInfo[i - 1].mTotalSize;
            }
            mComponentMemoryInfo[i].mComponentSize = componentInfo.size;
            mComponentMemoryInfo[i].mTotalSize = componentInfo.size * MaxEntityNum;
        }
    }

  protected:
    ArchetypeComponentMemoryInfo GetComponentMemoryInfo( usize index )
    {
        return mComponentMemoryInfo[index];
    }

    // 先设定到 16 kb
    inline usize GetArchetypeChunkDataSize()
    {
        return 16 * 1024;
    }

  public:
    ComponentIdSet mComponentIdSet;
    // memory offset and size
    std::vector<ArchetypeComponentMemoryInfo> mComponentMemoryInfo;

    // 应该是一个数组
    std::vector<ArchetypeChunk> mArchetypeChunks;
    usize MaxEntityNum = 0;
};
