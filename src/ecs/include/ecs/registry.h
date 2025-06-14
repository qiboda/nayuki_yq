#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include "ecs/archetype/archetype_manager.h"
#include "ecs/commands/command_manager.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/features/feature.h"
#include "ecs/features/feature_manager.h"
#include "ecs/schedule/schedule_manager.h"
#include "ecs/systems/system_manager.h"

// 实体，组件的管理类。等价于 ECS 中的 World
class ECS_API Registry
{
    friend class EntityCommand;
    friend class EntityInstanceCommand;

  public:
    Registry();

  public:
    template <IsFeature T>
    void AddFeature( T&& feature )
    {
        if ( mFeatureManager->ExistFeature<T>() == false )
        {
            feature.Build( this );
            mFeatureManager->AddedFeature( std::forward<T>( feature ) );
        }
    }

  public:
    void ConfigurePhase( PhaseConfigure&& configure )
    {
        mScheduleManager->ConfigSchedulePhase( std::forward<PhaseConfigure>( configure ) );
    }

  public:
    FeatureManager* mFeatureManager = nullptr;
    EntityManager* mEntityManager = nullptr;
    CommandManager* mCommandManager = nullptr;
    ArchetypeManager* mArchetypeManager = nullptr;
    ScheduleManager* mScheduleManager = nullptr;
};
