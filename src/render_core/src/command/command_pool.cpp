#include <render_core/render_core.h>
#include <render_core/command/command_pool.h>

void CommandPool::Initialize()
{
    CreateCommandPool();
}

void CommandPool::CleanUp()
{
    DestroyCommandPool();
}

void CommandPool::CreateCommandPool()
{
    NY_PRE_CONDITION( mDevice, "" )

    // create a CommandPool to allocate a CommandBuffer from
    vk::CommandPool commandPool = mDevice->createCommandPool(
        vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), graphicsQueueFamilyIndex ) );
    mCommandPool = vk::SharedCommandPool( commandPool, mDevice );
}

void CommandPool::DestroyCommandPool()
{
}

void CommandPool::SetDevice( vk::SharedDevice device )
{
    mDevice = device;
}
