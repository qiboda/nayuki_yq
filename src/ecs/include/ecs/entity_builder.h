#pragma once

#include "ecs/entity.h"
#include "ecs/registry.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <IsEntityConcept T>
struct EntityBuilder
{
    T entity;
    std::shared_ptr<Registry> registry;

    // 添加组件
    void add( Component *component )
    {
        registry->AddComponent( entity, component );
    }

    // 移除组件
    void remove( Component *component )
    {
        registry->RemoveComponent( entity, component );
    }

    // 销毁组件
    void destroy()
    {
        registry->DestroyEntity( entity );
    }
};