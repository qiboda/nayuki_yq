
module ecs;

import std;

Registry::Registry()
    : mEntityManager( std::make_shared<EntityManager>() )
    , mArchetypeManager( std::make_shared<ArchetypeManager>() )
    , mScheduleManager( std::make_shared<ScheduleManager>() )
    , mFeatureManager( std::make_shared<FeatureManager>() )
    , mCommandManager( std::make_shared<CommandManager>() )
{
}
