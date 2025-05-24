#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include <ecs/registry.h>
#include <memory>
#include <stack>

// 所有对实体和组件的修改操作都需要在主线程中进行。因此在非主线程的操作，都进行buffer。
// 后续如果有访问到这些组件和实体的操作需要执行，则立即触发buffer。
class ECS_API CommandBufferBase
{
  public:
    CommandBufferBase()
    {
    }

    virtual ~CommandBufferBase() = default;

  public:
    virtual void Execute( Registry *registry ) = 0;
    virtual usize GetSize() const = 0;
};

class CommandContext
{
  public:
    CommandContext( Registry *registry )
        : registry( registry )
    {
    }

  public:
    Registry *GetRegistry()
    {
        return registry;
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
    Registry *registry = nullptr;

    std::stack<std::weak_ptr<CommandBase>> mCommandStack;
};

/// 轻量类，可以很容易的拷贝
class ECS_API CommandBase : public std::enable_shared_from_this<CommandBase>
{
  public:
    CommandBase( std::shared_ptr<CommandContext> commandContext )
        : mCommandContext( commandContext )
    {
    }

    ~CommandBase()
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
    Registry *GetRegistry()
    {
        return mCommandContext->GetRegistry();
    }

  private:
    std::shared_ptr<CommandContext> mCommandContext;
};
