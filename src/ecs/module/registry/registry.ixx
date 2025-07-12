module;

#include "module_export.h"

export module ecs.registry;

import ecs.commands;

import core;
import std;
import ecs.archetype.manager;
import ecs.entity.manager;
import ecs.features.manager;
import ecs.features.feature;
import ecs.schedule.manager;
import ecs.registry.icontext;

// 实体，组件的管理类。等价于 ECS 中的 World
export class ECS_API Registry
{
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
        mRegistryContext->GetScheduleManager()->ConfigSchedulePhase( std::forward<PhaseConfigure>( configure ) );
    }

  public:
    IRegistryContext* mRegistryContext = nullptr;

    FeatureManager* mFeatureManager = nullptr;
    CommandManager* mCommandManager = nullptr;
};
