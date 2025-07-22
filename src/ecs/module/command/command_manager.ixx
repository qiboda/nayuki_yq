module;

#include "module_export.h"

export module ecs:command_manager;

import :command;
import :command_queue;

import core;
import core.concepts.derived;
import std;

export class ECS_API CommandManager
{
  public:
    CommandManager()
    {
    }

  public:
    template <typename T>
    void QueueCommand_AnyThread( T&& CommandBuffer )
    {
        static_assert( IsStrictDerivedConcept<CommandBufferBase, T> );
        Queue.AddCommand_AnyThread( std::forward<T>( CommandBuffer ) );
    }

    void ExecuteCommands( Registry* registry )
    {
        Queue.ExecuteCommands( registry );
    }

  protected:
    CommandQueue Queue;
};