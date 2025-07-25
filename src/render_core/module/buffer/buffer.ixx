module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>
#include <core/macro/macro.h>

export module render_core.buffer;

import std;
import core;
import core.misc.iraii;

export class RENDER_CORE_API Buffer : public IRAII
{
  public:
    Buffer()
    {
    }

    virtual ~Buffer() override
    {
    }

  public:
    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

  public:
    void CreateBuffer( vk::BufferUsageFlags usage, usize size )
    {
        NY_ASSERT( mDevice )

        auto bufferCreateInfo = vk::BufferCreateInfo( vk::BufferCreateFlags(), size, usage );
        vk::Buffer uniformDataBuffer = mDevice->createBuffer( bufferCreateInfo );
        mBuffer = vk::SharedBuffer( uniformDataBuffer, mDevice );
    }

    void destroyBuffer()
    {
        if ( mBuffer )
        {
            mDevice->destroyBuffer( mBuffer.get() );
            mBuffer.reset();
        }
    }

  public:
    virtual void* GetBuffer() = 0;
    virtual size_t GetSize() = 0;

    virtual void* Map() = 0;

    virtual void Unmap() = 0;

    virtual void Resize( size_t size ) = 0;

    virtual void CopyTo( Buffer* buffer ) = 0;

    virtual void CopyFrom( Buffer* buffer ) = 0;

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedBuffer mBuffer;
};

RENDER_TYPE_WRAPPER( Buffer )