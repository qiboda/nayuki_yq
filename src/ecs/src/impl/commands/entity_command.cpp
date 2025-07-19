module ecs;

// import :registry_command;
// import :command_manager;
// import :entity_instance_command;

// import :entity_manager;
// import :entity;

import std;

void CreateEntityCommandBuffer::Execute( Registry* registry )
{
    registry->GetEntityManager()->AddEntity( mEntity );
    registry->GetArchetypeManager()->CreateEntity( mEntity );
}

void DestroyEntityCommandBuffer::Execute( Registry* registry )
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