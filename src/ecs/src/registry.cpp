
#include "ecs/registry.h"
#include "ecs/archetype/archetype_manager.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/commands/command_manager.h"

Registry::Registry()
    : mEntityManager( new EntityManager() )
    , mCommandManager( new CommandManager() )
{
}

void Registry::CreateEntity( Entity entity )
{
    mEntityManager->AddEntity( entity );
    mArchetypeManager->CreateEntity( entity );
}

void Registry::DestroyEntity( Entity entity )
{
    mArchetypeManager->DestroyEntity( entity );
    mEntityManager->RemoveEntity( entity );
}