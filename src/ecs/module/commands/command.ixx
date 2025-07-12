module;

#include "module_export.h"
#include <memory>
#include <stack>

export module ecs.commands:command;

import core;
import ecs.registry.icontext;

import :forward;

// 所有对实体和组件的修改操作都需要在主线程中进行。因此在非主线程的操作，都进行buffer。
// 后续如果有访问到这些组件和实体的操作需要执行，则立即触发buffer。
export class ECS_API CommandBufferBase
{
  public:
    CommandBufferBase()
    {
    }

    virtual ~CommandBufferBase() = default;

  public:
    virtual void Execute( IRegistryContext* registry ) = 0;
    virtual usize GetSize() const = 0;
};

export class ECS_API CommandContext
{
  public:
    CommandContext( IRegistryContext* registry, CommandManager* commandManager )
        : registry( registry )
        , commandManager( commandManager )
    {
    }

  public:
    IRegistryContext* GetRegistry()
    {
        return registry;
    }

    CommandManager* GetCommandManager()
    {
        return commandManager;
    }

    void PushCommand( std::weak_ptr<class CommandBase> command )
    {
        mCommandStack.push( std::move( command ) );
    }

    std::weak_ptr<CommandBase> PopCommand()
    {
        std::weak_ptr<CommandBase> command;
        if ( !mCommandStack.empty() )
        {
            command = mCommandStack.top();
            mCommandStack.pop();
        }
        return command;
    }

  protected:
    IRegistryContext* registry = nullptr;
    CommandManager* commandManager = nullptr;

    std::stack<std::weak_ptr<CommandBase>> mCommandStack;
};

/// 轻量类，可以很容易的拷贝
export class ECS_API CommandBase : public std::enable_shared_from_this<CommandBase>
{
  public:
    CommandBase( std::shared_ptr<CommandContext> commandContext )
        : mCommandContext( commandContext )
    {
    }

    virtual ~CommandBase()
    {
    }

  public:
    std::shared_ptr<CommandContext> GetCommandContext()
    {
        return mCommandContext;
    }

    void PushCommand( std::weak_ptr<CommandBase> command )
    {
        mCommandContext->PushCommand( std::move( command ) );
    }

    std::weak_ptr<CommandBase> PopCommand()
    {
        return mCommandContext->PopCommand();
    }

  public:
    CommandManager* GetCommandManager()
    {
        return mCommandContext->GetCommandManager();
    }

    IRegistryContext* GetRegistry()
    {
        return mCommandContext->GetRegistry();
    }

  private:
    std::shared_ptr<CommandContext> mCommandContext;
};
