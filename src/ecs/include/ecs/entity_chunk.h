#pragma once

#include "ecs/component.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <vector>

// 对齐到 64 bit，对齐到缓存行大小，避免触发缓存一致性机制（MESI协议）[“伪共享”（False Sharing）]
class ECS_API alignas( 64 ) EntityChunk
{
  public:
    EntityChunk();

  public:
    // 添加组件数据
    template <typename T>
    void AddComponentData( const T *component )
    {
        UNUSED_VAR( component );
        auto it = mComponentsData.find( ComponentTrait<T>::Id );

        it.second.push_back( std::vector<u8>() );
    }

  protected:
    // key is component id
    std::map<ComponentId, std::vector<u8>> mComponentsData;
};
