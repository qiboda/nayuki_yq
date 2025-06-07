#pragma once

#include "ecs/commands/command.h"
#include "ecs/commands/registry_command.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API CreateEntityCommandBuffer : public CommandBufferBase
{
  public:
    CreateEntityCommandBuffer( Entity entity )
        : CommandBufferBase()
        , mEntity( entity )
    {
    }

  public:
    virtual void Execute( Registry* registry ) override;

    virtual usize GetSize() const override
    {
        return sizeof( CreateEntityCommandBuffer );
    }

    Entity mEntity;
};

class ECS_API DestroyEntityCommandBuffer : public CommandBufferBase
{
  public:
    DestroyEntityCommandBuffer( Entity entity )
        : CommandBufferBase()
        , mEntity( entity )
    {
    }

  public:
    virtual void Execute( Registry* registry ) override;

    virtual usize GetSize() const override
    {
        return sizeof( DestroyEntityCommandBuffer );
    }

    Entity mEntity;
};

// 实体命令类 命令类应用命令是否可以多线程？
class ECS_API EntityCommand : public CommandBase
{
  public:
    EntityCommand( std::shared_ptr<CommandContext> commandContext )
        : CommandBase( commandContext )
    {
    }

  public:
    std::shared_ptr<class EntityInstanceCommand> Create();

    std::shared_ptr<RegistryCommand> Destroy( Entity entity );

    // 回退到上一级
    std::shared_ptr<RegistryCommand> End()
    {
        return std::static_pointer_cast<RegistryCommand>( PopCommand().lock() );
    }
};
