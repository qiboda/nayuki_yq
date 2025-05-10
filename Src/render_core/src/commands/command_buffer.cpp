#include <render_core/render_core.h>
#include <render_core/commands/command_buffer.h>

CommandBuffer::CommandBuffer()
{
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::Initialize()
{
    CreateCommandBuffer();
}

void CommandBuffer::CleanUp()
{
    DestroyCommandBuffer();
}

void CommandBuffer::BeginCommandBuffer( const vk::CommandBufferBeginInfo &beginInfo )
{
    mCommandBuffer->begin( beginInfo );
}

void CommandBuffer::EndCommandBuffer()
{
    mCommandBuffer->end();
}

void CommandBuffer::CreateCommandBuffer()
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

void CommandBuffer::DestroyCommandBuffer()
{
    mDevice->freeCommandBuffers( mCommandPool.get(), mCommandBuffer.get() );
}
