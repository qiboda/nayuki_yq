module;

#include "core/misc/raii.h"
#include "render_core/render_core.h"
#include <module_export.h>

export module render_core.command_pool;

export class RENDER_CORE_API CommandPool : public IRAII
{
  public:
    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
        DestroyCommandPool();
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    void SetGraphicsQueueFamilyIndex( u32 index )
    {
        graphicsQueueFamilyIndex = index;
    }

  private:
    void CreateCommandPool()
    {
        NY_PRE_CONDITION( mDevice, "" )

        // create a CommandPool to allocate a CommandBuffer from
        vk::CommandPool commandPool = mDevice->createCommandPool(
            vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), graphicsQueueFamilyIndex ) );
        mCommandPool = vk::SharedCommandPool( commandPool, mDevice );
    }

    void DestroyCommandPool()
    {
    }

  private:
    vk::SharedDevice mDevice;
    vk::SharedCommandPool mCommandPool;

    u32 graphicsQueueFamilyIndex = 0;
};

RENDER_TYPE_WRAPPER( CommandPool )