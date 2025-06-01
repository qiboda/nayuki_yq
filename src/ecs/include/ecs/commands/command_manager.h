#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include <utility>
#include "ecs/commands/command.h"
#include "ecs/commands/command_queue.h"

class ECS_API CommandManager
{
  public:
    CommandManager();

  public:
    template <typename T>
    void QueueCommand_AnyThread( T&& CommandBuffer )
    {
        static_assert( std::is_base_of_v<CommandBufferBase, T> );
        Queue.AddCommand_AnyThread( std::forward<T>( CommandBuffer ) );
    }

    void ExecuteCommands( Registry* registry )
    {
        Queue.ExecuteCommands( registry );
    }

  protected:
    CommandQueue Queue;
};