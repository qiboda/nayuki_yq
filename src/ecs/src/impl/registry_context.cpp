
module ecs.registry.context;

import ecs.archetype.manager;
import ecs.entity.manager;

RegistryContext::RegistryContext()
    : mEntityManager( new EntityManager() )
    , mArchetypeManager( new ArchetypeManager() )
    , mScheduleManager( std::make_shared<ScheduleManager>() )
{
}
