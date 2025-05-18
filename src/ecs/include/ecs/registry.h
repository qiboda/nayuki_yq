#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

#include <ecs/entity.h>

// 实体，组件的管理类
class ECS_API Registry
{
  public:
    Registry();

  public:
    bool CreateEntity();

    bool DestroyEntity( Entity entity );

    void AddComponent( Entity entity, struct Component *component );

    void RemoveComponent( Entity entity, Component *component );
};