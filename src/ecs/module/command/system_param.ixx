module;

#include "module_export.h"

export module ecs:command_system_param;

import :command;
import :registry_command;
import :registry;

import std;
import :system_param;
import core;

export class ECS_API CommandsState : public SystemParamState
{
  public:
    CommandsState()
    {
    }

    void Init( Registry* registry, std::shared_ptr<CommandManager> commandManager )
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