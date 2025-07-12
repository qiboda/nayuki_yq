module ecs.commands;

import :registry_command;
import :manager;
import :entity_instance_command;

import std;
import ecs.entity.manager;
import ecs.registry.icontext;
import ecs.entity.entity;

void CreateEntityCommandBuffer::Execute( IRegistryContext* registry )
{
    registry->GetEntityManager()->AddEntity( mEntity );
    registry->GetArchetypeManager()->CreateEntity( mEntity );
}

void DestroyEntityCommandBuffer::Execute( IRegistryContext* registry )
{
    registry->GetEntityManager()->RemoveEntity( mEntity );
    registry->GetArchetypeManager()->DestroyEntity( mEntity );
}

std::shared_ptr<EntityInstanceCommand> EntityCommand::Create()
{
    Entity entity = GetRegistry()->GetEntityManager()->RequestCreateEntity_AnyThread();
    GetCommandManager()->QueueCommand_AnyThread( CreateEntityCommandBuffer( entity ) );

    PushCommand( weak_from_this() );
    return std::make_shared<EntityInstanceCommand>( GetCommandContext(), entity );
}

std::shared_ptr<RegistryCommand> EntityCommand::Destroy( Entity entity )
{
    GetCommandManager()->QueueCommand_AnyThread( DestroyEntityCommandBuffer( entity ) );

    return std::static_pointer_cast<RegistryCommand>( PopCommand().lock() );
}

std::shared_ptr<RegistryCommand> EntityCommand::End()
{
    return std::static_pointer_cast<RegistryCommand>( PopCommand().lock() );
}