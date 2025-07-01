module;

#include <core/core.h>
#include <render_core/render_core.h>

export module render_core.device_memory;

export class RENDER_CORE_API DeviceMemory : public IRAII
{
  public:
    DeviceMemory()
    {
    }

    virtual ~DeviceMemory() override
    {
    }

  public:
#pragma region "RAII"

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

#pragma endregion

  public:
    virtual void AllocateMemory( const vk::MemoryAllocateInfo& allocateInfo )
    {
        NY_ASSERT( mDevice )

        vk::DeviceMemory deviceMemory = mDevice->allocateMemory( allocateInfo );
        mDeviceMemory = vk::SharedDeviceMemory( deviceMemory, mDevice );
    }

    virtual void FreeMemory()
    {
        mDevice->freeMemory( mDeviceMemory.get() );
    }

    virtual void BindBuffer( vk::SharedBuffer buffer )
    {
        UNUSED_VARS( buffer );
    }

    virtual void BindImage( vk::SharedImage image )
    {
        UNUSED_VARS( image );
    }

    virtual void UnbindBuffer()
    {
    }

    virtual void UnbindImage()
    {
    }

  public:
    virtual void* GetMemory() const
    {
        return nullptr;
    }

    virtual usize GetSize() const
    {
        return 0u;
    }

    virtual void* MapMemory()
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