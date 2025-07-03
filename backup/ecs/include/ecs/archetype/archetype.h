#pragma once

#include "core/macro/macro.h"
#include "ecs/archetype/archetype_chunk.h"
#include "ecs/archetype/define.h"
#include "ecs/components/component_info.h"
#include "ecs/entity/entity.h"
#include "module_export.h"
#include <utility>
#include <vector>

// 组件id组合
class ECS_API Archetype
{
  public:
    using ChunkIterator = typename std::vector<ArchetypeChunk>::iterator;

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
        for ( auto& archetypeChunk : mArchetypeChunks )
        {
            NY_ASSERT( archetypeChunk.ContainEntity( entity ) == false )
            if ( archetypeChunk.CanAddEntity() )
            {
                auto memoryInfo = GetComponentMemoryInfo( 0 );
                archetypeChunk.AddEntity( entity, memoryInfo );
                isAdded = true;
                break;
            }
        }

        if ( isAdded == false )
        {
            AddNewArchetypeChunk();
            auto memoryInfo = GetComponentMemoryInfo( 0 );
            mArchetypeChunks.back().AddEntity( entity, memoryInfo );
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
        ArchetypeChunk* archetypeChunk = GetEntityArchetypeChunk( entity );
        if ( archetypeChunk )
        {
            archetypeChunk->DestroyEntity( entity, mComponentMemoryInfos );
        }
    }

    void MoveEntity( Entity entity, Archetype* destArchetype, ArchetypeChunk* destArchetypeChunk );

    ArchetypeChunk* GetEntityArchetypeChunk( Entity entity )
    {
        for ( auto& archetypeChunk : mArchetypeChunks )
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
    void AddEntityComponents( Archetype* oldArchetype, Entity entity, T&&... components )
    {
        ArchetypeChunk* newArchetypeChunk = GetEntityArchetypeChunk( entity );
        NY_ASSERT( newArchetypeChunk != nullptr )

        oldArchetype->MoveEntity( entity, this, newArchetypeChunk );
        AddEntityComponentsWithIndex( entity,
                                      std::make_index_sequence<sizeof...( T )>{},
                                      std::forward<T>( components )... );
    }

    template <IsComponentConcept... T>
    void AddEntityComponents( Entity entity, T&&... components )
    {
        AddEntityComponentsWithIndex( entity,
                                      std::make_index_sequence<sizeof...( T )>{},
                                      std::forward<T>( components )... );
    }

  protected:
    template <IsComponentConcept... T, usize... I>
    void AddEntityComponentsWithIndex( Entity entity, std::index_sequence<I...>, T&&... components )
    {
        ArchetypeChunk* curArchetypeChunk = GetEntityArchetypeChunk( entity );
        if ( curArchetypeChunk )
        {

            ( curArchetypeChunk->AddComponentData(
                  entity,
                  GetComponentMemoryInfoByComponentId( ComponentTypeRegistry::Get<T>() ),
                  std::forward<T>( components ) ),
              ... );
        }
    }

  public:
    template <IsComponentConcept... T>
    void ReplaceEntityComponents( Entity entity, T&&... components )
    {
        ReplaceEntityComponentsWithIndex( entity,
                                          std::make_index_sequence<sizeof...( T )>{},
                                          std::forward<T>( components )... );
    }

  protected:
    template <IsComponentConcept... T, usize... I>
    void ReplaceEntityComponentsWithIndex( Entity entity, std::index_sequence<I...> index, T&&... components )
    {
        ArchetypeChunk* curArchetypeChunk = GetEntityArchetypeChunk( entity );
        if ( curArchetypeChunk )
        {
            ( curArchetypeChunk->ReplaceComponentData( entity,
                                                       GetComponentMemoryInfo( index ),
                                                       std::forward<T>( components ) ),
              ... );
        }
    }

  public:
    template <IsComponentConcept... T>
    void RemoveEntityComponents( Archetype* oldArchetype, Entity entity, T&&... components )
    {
        ( UNUSED_VARS( components ), ... );

        ArchetypeChunk* newArchetypeChunk = this->GetEntityArchetypeChunk( entity );
        NY_ASSERT( newArchetypeChunk != nullptr )

        oldArchetype->MoveEntity( entity, this, newArchetypeChunk );
    }

  public:
    void Init( ComponentIdSet&& componentIdSet )
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

        if ( cumulativeSize == 0 )
        {
            MaxEntityNum = 0;
            return;
        }

        usize totalSize = GetArchetypeChunkDataSize();
        MaxEntityNum = totalSize / cumulativeSize;

        mComponentMemoryInfos.resize( mComponentIdSet.Get().size() );
        for ( usize i = 0; i < mComponentIdSet.Get().size(); ++i )
        {
            auto componentInfo = ComponentTypeRegistry::GetComponentInfo( mComponentIdSet.Get()[i] );
            if ( i == 0 )
            {
                mComponentMemoryInfos[i].mTotalOffset = 0;
            }
            else
            {
                mComponentMemoryInfos[i].mTotalOffset =
                    mComponentMemoryInfos[i - 1].mTotalOffset + mComponentMemoryInfos[i - 1].mTotalSize;
            }
            mComponentMemoryInfos[i].mComponentSize = componentInfo.size;
            mComponentMemoryInfos[i].mTotalSize = componentInfo.size * MaxEntityNum;
        }
    }

  protected:
    ArchetypeComponentMemoryInfo GetComponentMemoryInfo( usize index )
    {
        return mComponentMemoryInfos[index];
    }

    ArchetypeComponentMemoryInfo GetComponentMemoryInfoByComponentId( ComponentId id )
    {
        auto maybe_index = mComponentIdSet.IndexOf( id );
        NY_ASSERT( maybe_index.has_value() );
        auto index = maybe_index.value();
        return mComponentMemoryInfos[index];
    }

    // 先设定到 16 kb
    inline usize GetArchetypeChunkDataSize()
    {
        return 16 * 1024;
    }

  public:
    inline const ComponentIdSet& GetComponentIdSet() const
    {
        return mComponentIdSet;
    }

    template <typename... T>
        requires IsComponentSetConcept<T...>
    std::vector<ArchetypeComponentMemoryInfo> GetMemoryInfo( const ComponentIdSet& componentIdSet,
                                                             const std::vector<ArchetypeComponentMemoryInfo>& infos )
    {
        auto newComponentIdSet = ComponentTypeRegistry::GetComponentIdSet<T...>();
        std::array<usize, sizeof...( T )> componentIdIndices;
        for ( auto&& [i, id] : newComponentIdSet | ranges::views::enumerate )
        {
            auto index = componentIdSet.IndexOf( id );
            if ( index )
            {
                componentIdIndices[i] = index.value();
            }
        }

        std::vector<ArchetypeComponentMemoryInfo> newInfos;
        newInfos.resize( sizeof...( T ) );
        for ( auto&& [i, index] : componentIdIndices | ranges::views::enumerate )
        {
            newInfos[i] = infos[index];
        }
        return newInfos;
    }

  public:
    template <typename... T>
        requires IsComponentSetConcept<T...>
    class ComponentSetIterator
    {
      public:
        ComponentSetIterator()
        {
        }

        ComponentSetIterator( Archetype* archetype )
            : mArchetype( archetype )
        {
        }

        ComponentSetIterator( const ComponentSetIterator& iterator )
            : mArchetype( iterator.mArchetype )
            , mChunkIterator( iterator.mChunkIterator )
            , mComponentSetIterator( iterator.mComponentSetIterator )
        {
        }

        ComponentSetIterator<T...>& begin()
        {
            mChunkIterator = mArchetype->begin();
            if ( mChunkIterator != mArchetype->end() )
            {
                auto infos =
                    mArchetype->GetMemoryInfo<T...>( mArchetype->mComponentIdSet, mArchetype->mComponentMemoryInfos );
                mComponentSetIterator = mChunkIterator->ComponentSetIter<T...>( infos );
            }
            else
            {
                end();
            }
            return *this;
        }

        ComponentSetIterator<T...>& end()
        {
            mChunkIterator = mArchetype->end();
            mComponentSetIterator = std::nullopt;
            return *this;
        }

        void operator=( const ComponentSetIterator& other )
        {
            this->mArchetype = other.mArchetype;
            this->mChunkIterator = other.mChunkIterator;
            this->mComponentSetIterator = other.mComponentSetIterator;
        }

        // ++a
        ComponentSetIterator& operator++()
        {
            mComponentSetIterator++;
            while ( mComponentSetIterator.IsEnd() )
            {
                ++mChunkIterator;
                if ( mChunkIterator != mArchetype->end() )
                {
                    mComponentSetIterator = mChunkIterator->ComponentSetIter<T...>( mArchetype->mComponentIdSet,
                                                                                    mArchetype->mComponentMemoryInfos );
                }
                else
                {
                    mComponentSetIterator = std::nullopt;
                    break;
                }
            }
            return *this;
        }

        // a++
        ComponentSetIterator operator++( i32 )
        {
            ComponentSetIterator old = *this;
            if ( mComponentSetIterator.has_value() )
            {
                auto& iter = mComponentSetIterator.value();
                iter++;
                while ( iter.IsEnd() )
                {
                    ++mChunkIterator;
                    if ( mChunkIterator != mArchetype->end() )
                    {
                        auto infos = mArchetype->GetMemoryInfo<T...>( mArchetype->mComponentIdSet,
                                                                      mArchetype->mComponentMemoryInfos );
                        mComponentSetIterator = mChunkIterator->ComponentSetIter<T...>( infos );
                    }
                    else
                    {
                        mComponentSetIterator = std::nullopt;
                        break;
                    }
                }
            }
            return old;
        }

        bool operator==( const ComponentSetIterator& other ) const
        {
            return mArchetype == other.mArchetype && mChunkIterator == other.mChunkIterator &&
                   mComponentSetIterator == other.mComponentSetIterator;
        }

        std::tuple<T&...> operator*()
        {
            return mComponentSetIterator.value().GetComponents();
        }

        std::tuple<T&...> operator*() const
        {
            return mComponentSetIterator.value().GetComponents();
        }

        bool IsEnd() const
        {
            return mChunkIterator == mArchetype->mArchetypeChunks.end();
        }

        bool IsValid() const
        {
            return mArchetype != nullptr;
        }

      protected:
        Archetype* mArchetype = nullptr;
        ChunkIterator mChunkIterator;
        std::optional<ArchetypeChunk::ComponentSetIterator<T...>> mComponentSetIterator;
    };

    ChunkIterator begin()
    {
        return mArchetypeChunks.begin();
    }

    ChunkIterator end()
    {
        return mArchetypeChunks.end();
    }

    template <typename... T>
        requires IsComponentSetConcept<T...>
    ComponentSetIterator<T...> ComponentSetIter()
    {
        return ComponentSetIterator<T...>( this );
    }

    inline usize GetTotalEntityNum() const
    {
        usize totalNum = 0;
        for ( auto& chunk : mArchetypeChunks )
        {
            totalNum += chunk.GetEntityNum();
        }
        return totalNum;
    }

  protected:
    ComponentIdSet mComponentIdSet;
    // memory offset and size
    std::vector<ArchetypeComponentMemoryInfo> mComponentMemoryInfos;

    // 应该是一个数组
    std::vector<ArchetypeChunk> mArchetypeChunks;
    usize MaxEntityNum = 0;
};
