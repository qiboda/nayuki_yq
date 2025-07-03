module;

// #include "core/misc/raii.h"
import core.misc;
#include "module_export.h"
#include "render_core/render_core.h"

export module render_core.command_buffer;

export class RENDER_CORE_API CommandBufferBase : public IRAII
{
  public:
    CommandBufferBase()
    {
    }

    virtual ~CommandBufferBase() override
    {
    }

  public:
    virtual void Initialize() override
    {
        CreateCommandBuffer();
    }

    virtual void CleanUp() override
    {
        DestroyCommandBuffer();
    }

  public:
    void CreateCommandBuffer()
    {
        NY_PRE_CONDITION( mDevice, "" )
        NY_PRE_CONDITION( mCommandBuffer, "" )

        vk::CommandBuffer commandBuffer =
            mDevice
                ->allocateCommandBuffers(
                    vk::CommandBufferAllocateInfo( mCommandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) )
                .front();
        mCommandBuffer = vk::SharedCommandBuffer( commandBuffer, mDevice, mCommandPool );
    }

    void DestroyCommandBuffer()
    {
        mDevice->freeCommandBuffers( mCommandPool.get(), mCommandBuffer.get() );
    }

  public:
    void BeginCommandBuffer( const vk::CommandBufferBeginInfo& beginInfo )
    {

        mCommandBuffer->begin( beginInfo );
    }

    void EndCommandBuffer()
    {
        mCommandBuffer->end();
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    void SetCommandPool( vk::SharedCommandPool commandPool )
    {
        mCommandPool = commandPool;
    }

  private:
    vk::SharedCommandPool mCommandPool;
    vk::SharedDevice mDevice;
    vk::SharedCommandBuffer mCommandBuffer;
};

RENDER_TYPE_WRAPPER( CommandBufferBase )