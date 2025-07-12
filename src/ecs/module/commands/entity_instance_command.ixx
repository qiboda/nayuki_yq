module;

#include "module_export.h"

export module ecs.commands:entity_instance_command;

import :command;
import :entity_command;
import :manager;

import std;
import core.type;
import ecs.components.concepts;
import ecs.entity.entity;
import ecs.registry.icontext;

export template <IsComponentConcept... T>
class AddComponentCommandBuffer : public CommandBufferBase
{
  public:
    AddComponentCommandBuffer( Entity entity, T&&... component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents{ std::forward<T>( component )... }
    {
    }

  public:
    virtual void Execute( IRegistryContext* registry ) override
    {
        std::apply(
            [&]( auto&&... comps )
            {
                registry->GetArchetypeManager()->AddComponentData( mEntity, std::forward<T>( comps )... );
            },
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

export template <IsComponentConcept... T>
class RemoveComponentCommandBuffer : public CommandBufferBase
{
  public:
    RemoveComponentCommandBuffer( Entity entity, T&&... component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents( std::forward<T>( component )... )
    {
    }

  public:
    virtual void Execute( IRegistryContext* registry ) override
    {
        std::apply(
            [&]( auto&&... comps )
            {
                registry->GetArchetypeManager()->RemoveComponentData( mEntity, std::forward<T>( comps )... );
            },
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

export template <IsComponentConcept... T>
class ReplaceComponentCommandBuffer : public CommandBufferBase
{
  public:
    ReplaceComponentCommandBuffer( Entity entity, T&&... component )
        : CommandBufferBase()
        , mEntity( entity )
        , mComponents( std::forward<T>( component )... )
    {
    }

  public:
    virtual void Execute( IRegistryContext* registry ) override
    {
        std::apply(
            [&]( auto&&... comps )
            {
                registry->GetArchetypeManager()->ReplaceComponentData( mEntity, std::forward<T>( comps )... );
            },
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

export class ECS_API EntityInstanceCommand : public CommandBase
{
  public:
    EntityInstanceCommand( std::shared_ptr<CommandContext> commandContext, Entity entity )
        : CommandBase( commandContext )
        , mEntity( entity )
    {
    }

  public:
    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> AddComponent( T&&... components )
    {
        QueueCommand( AddComponentCommandBuffer<T...>( mEntity, std::forward<T>( components )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> ReplaceComponent( T&&... component )
    {
        QueueCommand( ReplaceComponentCommandBuffer<T...>( mEntity, std::forward<T>( component )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

    template <IsComponentConcept... T>
    std::shared_ptr<EntityInstanceCommand> RemoveComponent( T&&... component )
    {
        QueueCommand( RemoveComponentCommandBuffer<T...>( mEntity, std::forward<T>( component )... ) );
        return std::static_pointer_cast<EntityInstanceCommand>( shared_from_this() );
    }

  protected:
    template <typename T>
    void QueueCommand( T&& base )
    {
        GetCommandContext()->GetCommandManager()->QueueCommand_AnyThread( std::forward<T>( base ) );
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