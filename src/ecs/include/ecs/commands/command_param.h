#pragma once

#include "ecs/commands/registry_command.h"
#include "ecs/registry.h"
#include "ecs/systems/system_param.h"
#include <core/core.h>

class ECS_API CommandsState : public SystemParamState
{
  public:
    CommandsState()
    {
    }

    void Init( Registry* registry )
    {
        command = RegistryCommand::Get( registry );
    }

    std::shared_ptr<RegistryCommand>& operator()()
    {
        return command;
    }

  protected:
    std::shared_ptr<RegistryCommand> command;
};

class ECS_API Commands : public SystemParam
{
  public:
    using State = CommandsState;

  public:
    Commands( CommandsState& state )
        : mState( state )
    {
    }

    static Commands From( Registry* registry, State& state )
    {
        state.Init( registry );
        return Commands( state );
    }

  public:
    std::shared_ptr<RegistryCommand>& operator()()
    {
        return mState();
    }

  protected:
    State& mState;
};