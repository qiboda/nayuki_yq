
#include "ecs/commands/entity_command.h"
#include "ecs/commands/registry_command.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/commands/entity_instance_command.h"
#include "ecs/commands/command_manager.h"
#include <memory>

void CreateEntityCommandBuffer::Execute( Registry *registry )
{
    registry->mEntityManager->AddEntity( mEntity );
}

void DestroyEntityCommandBuffer::Execute( Registry *registry )
{
    registry->mEntityManager->RemoveEntity( mEntity );
}

std::shared_ptr<EntityInstanceCommand> EntityCommand::Create()
{
    Entity entity = GetRegistry()->mEntityManager->RequestCreateEntity_AnyThread();
    GetRegistry()->mCommandManager->QueueCommand_AnyThread( CreateEntityCommandBuffer( entity ) );

    PushCommand( weak_from_this() );
    return std::make_shared<EntityInstanceCommand>( GetCommandContext(), entity );
}

std::shared_ptr<RegistryCommand> EntityCommand::Destroy( Entity entity )
{
    auto registry = GetRegistry();
    registry->mCommandManager->QueueCommand_AnyThread( DestroyEntityCommandBuffer( entity ) );

    return std::static_pointer_cast<RegistryCommand>( PopCommand().lock() );
}