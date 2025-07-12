
module ecs.commands;

import :entity_command;
import std;

std::shared_ptr<EntityCommand> RegistryCommand::Entity()
{
    PushCommand( weak_from_this() );
    return std::make_shared<EntityCommand>( GetCommandContext() );
}
