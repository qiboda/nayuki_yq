#pragma once

#include "ecs/commands/command.h"
#include "ecs/registry.h"
#include <core/core.h>
#include <memory>

class ECS_API RegistryCommand : public CommandBase
{
  public:
    RegistryCommand( Registry* registry )
        : CommandBase( std::make_unique<CommandContext>( registry ) )
    {
    }

    static std::shared_ptr<RegistryCommand> Get( Registry* registry )
    {
        return std::make_shared<RegistryCommand>( registry );
    }

  public:
    std::shared_ptr<class EntityCommand> Entity();
};