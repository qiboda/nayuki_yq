#pragma once

#include "ecs/commands/command.h"
#include <core/core.h>

class ECS_API CommandQueue
{
  public:
    CommandQueue();

  public:
    template <typename T>
    void AddCommand_AnyThread( T&& command )
    {
        static_assert( IsStrictDerivedConcept<CommandBufferBase, T> );

        usize base = mBuffer.size();
        mBuffer.resize( base + sizeof( T ) );
        new ( mBuffer.data() + base ) T( std::forward<T>( command ) );
    }

    void ExecuteCommands( Registry* registry )
    {
        usize size = mBuffer.size();
        usize base = 0;
        while ( base < size )
        {
            auto commandBuffer = reinterpret_cast<CommandBufferBase*>( mBuffer.data() + base );
            commandBuffer->Execute( registry );
            base += commandBuffer->GetSize();
        }
        mBuffer.clear();
    }

  public:
    std::vector<u8> mBuffer;
};
