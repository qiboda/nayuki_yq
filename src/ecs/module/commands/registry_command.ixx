module;

#include "module_export.h"

export module ecs.commands:registry_command;

import :command;
import :manager;
import :entity_command;

import ecs.registry.icontext;
import core;
import std;

export class ECS_API RegistryCommand : public CommandBase
{
  public:
    RegistryCommand( IRegistryContext* registry, CommandManager* commandManager )
        : CommandBase( std::make_shared<CommandContext>( registry, commandManager ) )
    {
    }

    static std::shared_ptr<RegistryCommand> Get( IRegistryContext* registry, CommandManager* commandManager )
    {
        return std::make_shared<RegistryCommand>( registry, commandManager );
    }

  public:
    std::shared_ptr<EntityCommand> Entity();
};