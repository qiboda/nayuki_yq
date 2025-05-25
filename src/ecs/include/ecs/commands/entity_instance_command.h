#pragma once

#include "ecs/commands/command.h"
#include "ecs/commands/entity_command.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <memory>
#include <utility>
#include <ecs/commands/command_manager.h>

template <IsComponentConcept... T>
class AddComponentCommandBuffer : public CommandBufferBase
{
  public:
    AddComponentCommandBuffer( Entity entity, T &&...component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents{ std::forward<T>( component )... }
    {
    }

  public:
    virtual void Execute( Registry *registry ) override
    {
        std::apply( [&]( auto &&...comps )
                    { registry->mArchetypeManager->AddComponentData( mEntity, std::forward<T>( comps )... ); },
                    std::forward<std::tuple<T...>>( mComponents ) );
    }

    virtual usize GetSize() const override
    {
        return sizeof( AddComponentCommandBuffer );
    }

  public:
    Entity mEntity;
    std::tuple<T...> mComponents;
};

template <IsComponentConcept... T>
class RemoveComponentCommandBuffer : public CommandBufferBase
{
  public:
    RemoveComponentCommandBuffer( Entity entity, T &&...component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents( std::forward<T>( component )... )
    {
    }

  public:
    virtual void Execute( Registry *registry ) override
    {
        std::apply( [&]( auto &&...comps )
                    { registry->mArchetypeManager->RemoveComponentData( mEntity, std::forward<T>( comps )... ); },
                    std::forward<std::tuple<T...>>( mComponents ) );
    }

    virtual usize GetSize() const override
    {
        return sizeof( RemoveComponentCommandBuffer );
    }

  public:
    Entity mEntity;
    std::tuple<T...> mComponents;
};

template <IsComponentConcept... T>
class ReplaceComponentCommandBuffer : public CommandBufferBase
{
  public:
    ReplaceComponentCommandBuffer( Entity entity, T &&...component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents( std::forward<T>( component )... )
    {
    }

  public:
    virtual void Execute( Registry *registry ) override
    {
        std::apply( [&]( auto &&...comps )
                    { registry->mArchetypeManager->ReplaceComponentData( mEntity, std::forward<T>( comps )... ); },
                    std::forward<std::tuple<T...>>( mComponents ) );
    }

    virtual usize GetSize() const override
    {
        return sizeof( ReplaceComponentCommandBuffer );
    }

  public:
    Entity mEntity;
    std::tuple<T...> mComponents;
};

class ECS_API EntityInstanceCommand : public CommandBase
{
  public:
    EntityInstanceCommand( std::shared_ptr<CommandContext> commandContext, Entity entity )
        : CommandBase( commandContext )
        , mEntity( entity )
    {
    }

  public:
    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> AddComponent( T &&...components )
    {
        GetRegistry()->mCommandManager->QueueCommand_AnyThread(
            AddComponentCommandBuffer<T...>( mEntity, std::forward<T>( components )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> ReplaceComponent( T &&...component )
    {
        GetRegistry()->mCommandManager->QueueCommand_AnyThread(
            ReplaceComponentCommandBuffer<T...>( mEntity, std::forward<T>( component )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> RemoveComponent( T &&...component )
    {
        GetRegistry()->mCommandManager->QueueCommand_AnyThread(
            RemoveComponentCommandBuffer<T...>( mEntity, std::forward<T>( component )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

  public:
    // 回退到上一级
    std::shared_ptr<EntityCommand> End()
    {
        return std::static_pointer_cast<EntityCommand>( PopCommand().lock() );
    }

  protected:
    Entity mEntity;
};