#include <render_core/render_core.h>
#include <render_core/command/command_buffer.h>

CommandBufferBase::CommandBufferBase()
{
}

CommandBufferBase::~CommandBufferBase()
{
}

void CommandBufferBase::Initialize()
{
    CreateCommandBuffer();
}

void CommandBufferBase::CleanUp()
{
    DestroyCommandBuffer();
}

void CommandBufferBase::BeginCommandBuffer( const vk::CommandBufferBeginInfo &beginInfo )
{
    mCommandBuffer->begin( beginInfo );
}

void CommandBufferBase::EndCommandBuffer()
{
    mCommandBuffer->end();
}

void CommandBufferBase::CreateCommandBuffer()
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

void CommandBufferBase::DestroyCommandBuffer()
{
    mDevice->freeCommandBuffers( mCommandPool.get(), mCommandBuffer.get() );
}
