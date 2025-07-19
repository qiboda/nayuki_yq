module;

#include "module_export.h"

export module ecs:registry_command;

import :command;
import :command_manager;
import :entity_command;

import core;
import std;

export class ECS_API RegistryCommand : public CommandBase
{
  public:
    RegistryCommand( Registry* registry, std::shared_ptr<CommandManager> commandManager )
        : CommandBase( std::make_shared<CommandContext>( registry, commandManager ) )
    {
    }

    static std::shared_ptr<RegistryCommand> Get( Registry* registry, std::shared_ptr<CommandManager> commandManager )
    {
        return std::make_shared<RegistryCommand>( registry, commandManager );
    }

  public:
    std::shared_ptr<EntityCommand> Entity();
};