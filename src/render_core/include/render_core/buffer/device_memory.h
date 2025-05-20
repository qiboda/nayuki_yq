#pragma once

#include "core/macro/macro.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API DeviceMemory : public IRAII
{
  public:
    DeviceMemory();
    virtual ~DeviceMemory() override;

  public:
#pragma region "RAII"
    virtual void Initialize() override;
    virtual void CleanUp() override;
#pragma endregion

  public:
    virtual void AllocateMemory( const vk::MemoryAllocateInfo &allocateInfo );

    virtual void FreeMemory()
    {
        mDevice->freeMemory( mDeviceMemory.get() );
    }

    virtual void BindBuffer( vk::SharedBuffer buffer )
    {
        UNUSED_VAR( buffer );
    }

    virtual void BindImage( vk::SharedImage image )
    {
        UNUSED_VAR( image );
    }

    virtual void UnbindBuffer()
    {
    }

    virtual void UnbindImage()
    {
    }

  public:
    virtual void *GetMemory() const
    {
        return nullptr;
    }

    virtual usize GetSize() const
    {
        return 0u;
    }

    virtual void *MapMemory()
    {
        return nullptr;
    }

    virtual void UnmapMemory()
    {
    }

    virtual void Flush()
    {
    }

    virtual void Invalidate()
    {
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedDeviceMemory mDeviceMemory;
};

RENDER_TYPE_WRAPPER( DeviceMemory )