
#include "render_core/buffer/device_memory.h"

DeviceMemory::DeviceMemory()
{
}

DeviceMemory::~DeviceMemory()
{
}

void DeviceMemory::Initialize()
{
}

void DeviceMemory::CleanUp()
{
}

void DeviceMemory::AllocateMemory( const vk::MemoryAllocateInfo& allocateInfo )
{
    NY_ASSERT( mDevice )

    vk::DeviceMemory deviceMemory = mDevice->allocateMemory( allocateInfo );
    mDeviceMemory = vk::SharedDeviceMemory( deviceMemory, mDevice );
}
