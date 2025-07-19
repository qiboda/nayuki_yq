module;

#include "module_export.h"

export module ecs:registry;

import :feature_manager;
import :command_manager;

import core;
import std;
import :archetype_manager;
import :entity_manager;
import :schedule_manager;
import :schedule_phase;

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
        mScheduleManager->ConfigSchedulePhase( std::forward<PhaseConfigure>( configure ) );
    }

    std::shared_ptr<ArchetypeManager> GetArchetypeManager()
    {
        return mArchetypeManager;
    }

    std::shared_ptr<EntityManager> GetEntityManager()
    {
        return mEntityManager;
    }

    std::shared_ptr<ScheduleManager> GetScheduleManager()
    {
        return mScheduleManager;
    }

    std::shared_ptr<FeatureManager> GetFeatureManager()
    {
        return mFeatureManager;
    }

    std::shared_ptr<CommandManager> GetCommandManager()
    {
        return mCommandManager;
    }

  public:
    template <IsSchedulePhase T>
    void AddSystem( class ScheduleSystemNodeConfig&& config )
    {
        mScheduleManager->AddSystemNodeConfig<T>( std::move( config ) );
    }

    /// 添加一个系统集节点配置到调度图中。
    template <IsSchedulePhase T>
    void AddSystemSet( class ScheduleSystemSetNodeConfig&& config )
    {
        mScheduleManager->AddSystemSetNodeConfig<T>( std::move( config ) );
    }

    void Run()
    {
        mScheduleManager->Run( this );
    }

  public:
    std::shared_ptr<EntityManager> mEntityManager = nullptr;
    std::shared_ptr<ArchetypeManager> mArchetypeManager = nullptr;
    std::shared_ptr<ScheduleManager> mScheduleManager = nullptr;

    std::shared_ptr<FeatureManager> mFeatureManager = nullptr;
    std::shared_ptr<CommandManager> mCommandManager = nullptr;
};
