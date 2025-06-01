#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API Buffer : public IRAII
{
  public:
    Buffer();
    virtual ~Buffer() override;

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

  public:
    void CreateBuffer( vk::BufferUsageFlags usage, usize size );

    void destroyBuffer();

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