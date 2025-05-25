#pragma once

#include "ecs/archetype/define.h"
#include <core/memory/memory.h>
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include <core/minimal.h>
#include <cstring>
#include <ecs/minimal.h>
#include <iterator>
#include <utility>

// 对齐到 64 bit，对齐到缓存行大小，避免触发缓存一致性机制（MESI协议）[“伪共享”（False Sharing）]
class ECS_API ArchetypeChunk
{
  public:
    explicit ArchetypeChunk( usize MaxEntityNum, usize memorySize = 16 * 1024 )
        : MaxEntityNum( MaxEntityNum )
    {
        std::cout << "ArchetypeChunk::ArchetypeChunk " << memorySize << std::endl;
        mArchetypeComponentData = new u8[memorySize]();
    }

    ~ArchetypeChunk()
    {
        if ( mArchetypeComponentData )
        {
            delete[] mArchetypeComponentData;
        }
    }

  public:
    inline bool CanAddEntity() const
    {
        return mEntities.size() < MaxEntityNum;
    }

  public:
    inline void AddEntity( Entity entity )
    {
        mEntities.push_back( entity );
    }

    inline void DestroyEntity( Entity entity, const std::vector<ArchetypeComponentMemoryInfo> &memoryInfos )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        if ( it != mEntities.end() )
        {
            *it = std::move( mEntities.back() ); // 用最后一个元素覆盖它
            usize entityIndex = std::distance( mEntities.begin(), it );
            usize entityNum = mEntities.size();
            mEntities.pop_back(); // 移除最后一个

            for ( auto &&memoryInfo : memoryInfos )
            {
                u8 *removedComponentData =
                    mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
                u8 *lastComponentData =
                    mArchetypeComponentData + memoryInfo.mTotalOffset + entityNum * memoryInfo.mComponentSize;

                auto *component = reinterpret_cast<Component *>( removedComponentData );
                component->~Component();

                std::memcpy( removedComponentData, lastComponentData, memoryInfo.mComponentSize );
            }
        }
    }

    inline bool ContainEntity( Entity entity )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        return it != mEntities.end();
    }

  public:
    // 添加组件数据
    template <IsComponentConcept T>
    void AddComponentData( Entity entity, const ArchetypeComponentMemoryInfo &memoryInfo, T &&component )
    {
        NY_ASSERT( mArchetypeComponentData != nullptr );

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        NY_ASSERT( it != mEntities.end() );
        usize entityIndex = std::distance( mEntities.begin(), it );

        u8 *componentData = mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
        new ( componentData ) T( std::forward<T>( component ) );
    }

    template <IsComponentConcept T>
    void ReplaceComponentData( Entity entity, ArchetypeComponentMemoryInfo &memoryInfo, T &&newComponent )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        NY_ASSERT( it != mEntities.end() );
        usize entityIndex = std::distance( mEntities.begin(), it );

        u8 *componentData = mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;

        Component *component = reinterpret_cast<Component *>( componentData );
        component->~Component();

        new ( componentData ) T( std::forward<T>( newComponent ) );
    }

  public:
    template <IsComponentConcept T>
    T *GetComponents( const ArchetypeComponentMemoryInfo &memoryInfo )
    {
        u8 *componentBaseData = mArchetypeComponentData + memoryInfo.mTotalOffset;
        return reinterpret_cast<T *>( componentBaseData );
    }

    std::vector<u8 *> MoveOutEntity( Entity entity, const std::vector<ArchetypeComponentMemoryInfo> &memoryInfos )
    {
        usize entityTotalNum = mEntities.size();

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        if ( it == mEntities.end() )
        {
            return {};
        }

        size_t entityIndex = std::distance( mEntities.begin(), it );

        *it = std::move( mEntities.back() );
        mEntities.pop_back();

        std::vector<u8 *> movedComponents;
        movedComponents.reserve( memoryInfos.size() );
        for ( auto &&memoryInfo : memoryInfos )
        {
            u8 *movedComponent =
                mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
            u8 *lastComponent =
                mArchetypeComponentData + memoryInfo.mTotalOffset + ( entityTotalNum - 1 ) * memoryInfo.mComponentSize;

            Memory::Swap( movedComponent, lastComponent, memoryInfo.mComponentSize );
            movedComponents.push_back( lastComponent );
        }

        return movedComponents;
    }

    void MoveIntoEntityComponent( Entity entity,
                                  const std::vector<u8 *> &srcComponents,
                                  const usize srcIndex,
                                  const usize destIndex,
                                  const std::vector<ArchetypeComponentMemoryInfo> &memoryInfos )
    {
        u8 *srcComponentData = srcComponents[srcIndex];

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        usize entityIndex = std::distance( mEntities.begin(), it );
        u8 *destComponentData = mArchetypeComponentData + memoryInfos[destIndex].mTotalOffset +
                                entityIndex * memoryInfos[destIndex].mComponentSize;

        std::memcpy( destComponentData, srcComponentData, memoryInfos[destIndex].mComponentSize );
    }

  public:
    // TODO: 之后使用运行时配置
    // alignas( 64 )
    u8 *mArchetypeComponentData = nullptr;

    // 相同架构的所有实体
    std::vector<Entity> mEntities;
    usize MaxEntityNum = 0;
};
