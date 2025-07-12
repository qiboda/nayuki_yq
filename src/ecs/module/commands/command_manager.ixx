module;

#include "module_export.h"

export module ecs.commands:manager;

import :command;
import :queue;

import core;
import core.concepts.derived;
import std;
import ecs.registry.icontext;

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

    void ExecuteCommands( IRegistryContext* registry )
    {
        Queue.ExecuteCommands( registry );
    }

  protected:
    CommandQueue Queue;
};