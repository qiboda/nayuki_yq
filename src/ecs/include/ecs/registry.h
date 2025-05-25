#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include "ecs/archetype/archetype_manager.h"
#include "ecs/commands/command_manager.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"

// 实体，组件的管理类。等价于 ECS 中的 World
class ECS_API Registry
{
    friend class EntityCommand;
    friend class EntityInstanceCommand;

  public:
    Registry();

  public:
    EntityManager *mEntityManager = nullptr;
    CommandManager *mCommandManager = nullptr;
    ArchetypeManager *mArchetypeManager = nullptr;

  protected:
    void CreateEntity( Entity entity );

    void DestroyEntity( Entity entity );

  protected:
    template <IsComponentConcept T>
    void AddComponent( Entity entity, T &&component )
    {
        ComponentTypeRegistry::Register<T>();

        mArchetypeManager->AddComponentData<T>( entity, std::forward<T>( component ) );
    }
};
