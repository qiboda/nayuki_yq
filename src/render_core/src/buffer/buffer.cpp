
#include "render_core/buffer/buffer.h"
#include "core/macro/macro.h"

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::Initialize()
{
}

void Buffer::CleanUp()
{
}

void Buffer::CreateBuffer( vk::BufferUsageFlags usage, usize size )
{
    NY_ASSERT( mDevice )

    auto bufferCreateInfo = vk::BufferCreateInfo( vk::BufferCreateFlags(), size, usage );
    vk::Buffer uniformDataBuffer = mDevice->createBuffer( bufferCreateInfo );
    mBuffer = vk::SharedBuffer( uniformDataBuffer, mDevice );
}

void Buffer::destroyBuffer()
{
    if ( mBuffer )
    {
        mDevice->destroyBuffer( mBuffer.get() );
        mBuffer.reset();
    }
}
