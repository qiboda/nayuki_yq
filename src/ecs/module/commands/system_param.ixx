module;

#include "module_export.h"

export module ecs.commands.system;

import ecs.commands;

import std;
import ecs.registry.icontext;
import ecs.systems.params;
import core;
import ecs.registry;

export class ECS_API CommandsState : public SystemParamState
{
  public:
    CommandsState()
    {
    }

    void Init( IRegistryContext* registry, CommandManager* commandManager )
    {
        if ( !command )
        {
            command = RegistryCommand::Get( registry, commandManager );
        }
    }

    std::shared_ptr<RegistryCommand>& operator()()
    {
        return command;
    }

  protected:
    std::shared_ptr<RegistryCommand> command;
};

export class ECS_API Commands : public SystemParam
{
  public:
    using State = CommandsState;

  public:
    Commands( CommandsState& state )
        : mState( state )
    {
    }

    static Commands From( Registry* registry, State& state );

  public:
    std::shared_ptr<RegistryCommand>& operator()()
    {
        return mState();
    }

  protected:
    State& mState;
};