
#include "ecs/registry.h"
#include "ecs/archetype/archetype_manager.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/commands/command_manager.h"
#include "ecs/systems/system_manager.h"

Registry::Registry()
    : mFeatureManager( new FeatureManager() )
    , mEntityManager( new EntityManager() )
    , mCommandManager( new CommandManager() )
    , mArchetypeManager( new ArchetypeManager() )
    , mScheduleManager( new ScheduleManager() )
{
}
