
#include "render_core/device/device.h"

Device::Device()
{
}

Device::~Device()
{
    CleanUp();
}

void Device::CreateDevice( const vk::DeviceCreateInfo &deviceInfo )
{
    NY_ASSERT( mPhysicalDevice )

    vk::Device device = mPhysicalDevice->GetRaw().createDevice( deviceInfo );
    mDevice = vk::SharedDevice( device );
}

void Device::DestroyDevice()
{
    mDevice.reset();
}
