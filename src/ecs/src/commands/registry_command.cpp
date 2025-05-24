
#include "ecs/commands/registry_command.h"
#include "ecs/commands/entity_command.h"
#include <memory>

std::shared_ptr<EntityCommand> RegistryCommand::Entity()
{
    PushCommand( weak_from_this() );
    return std::make_shared<EntityCommand>( GetCommandContext() );
}
