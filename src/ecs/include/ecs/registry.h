#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include <ecs/component.h>

#include <ecs/entity/entity.h>

// 实体，组件的管理类。等价于 ECS 中的 World
class ECS_API Registry
{
  public:
    Registry();

  public:
    // 索引是 Component Id
    std::vector<struct ComponentInfo> mAllComponentInfo;
    class EntityManager *mEntityManager = nullptr;
    class CommandManager *mCommandManager = nullptr;

  public:
    bool CreateEntity();

    bool DestroyEntity( Entity entity );
};
