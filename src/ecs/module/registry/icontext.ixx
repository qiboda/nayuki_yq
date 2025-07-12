module;

#include "module_export.h"

export module ecs.registry.icontext;

import std;
import ecs.archetype.manager;
import ecs.entity.manager;
import ecs.schedule.manager;

export class ECS_API IRegistryContext
{
  public:
    virtual ~IRegistryContext() = default;

  public:
    virtual EntityManager* GetEntityManager() = 0;

    virtual ArchetypeManager* GetArchetypeManager() = 0;

    virtual std::shared_ptr<ScheduleManager> GetScheduleManager() = 0;
};