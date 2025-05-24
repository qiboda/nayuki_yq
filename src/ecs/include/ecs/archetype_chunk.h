#pragma once

#include "ecs/component.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <vector>

// 对齐到 64 bit，对齐到缓存行大小，避免触发缓存一致性机制（MESI协议）[“伪共享”（False Sharing）]
class ECS_API ArchetypeChunk
{
  public:
    ArchetypeChunk();

  public:
    // 添加组件数据
    template <typename T>
    void AddComponentData( const T *component )
    {
        ComponentId Id = ComponentTypeRegistry::Get<T>();

        UNUSED_VAR( component );
        // auto it = mComponentsData.find( ComponentTrait<T>::Id );

        // it.second.push_back( std::vector<u8>() );
    }

  protected:
    alignas( 64 ) void *mArchetypeComponentData;
};
