module;

#include "module_export.h"

export module ecs.registry.context;

import core;
import std;
import ecs.archetype.manager;
import ecs.entity.manager;
import ecs.schedule.manager;
import ecs.registry.icontext;

// 实体，组件的管理类。等价于 ECS 中的 World
export class ECS_API RegistryContext : public IRegistryContext
{
  public:
    RegistryContext();

  public:
    void ConfigurePhase( PhaseConfigure&& configure )
    {
        mScheduleManager->ConfigSchedulePhase( std::forward<PhaseConfigure>( configure ) );
    }

    virtual EntityManager* GetEntityManager() override
    {
        return mEntityManager;
    }

    virtual ArchetypeManager* GetArchetypeManager() override
    {
        return mArchetypeManager;
    }

    virtual std::shared_ptr<ScheduleManager> GetScheduleManager() override
    {
        return mScheduleManager;
    }

  public:
    EntityManager* mEntityManager = nullptr;
    ArchetypeManager* mArchetypeManager = nullptr;
    std::shared_ptr<ScheduleManager> mScheduleManager = nullptr;
};
