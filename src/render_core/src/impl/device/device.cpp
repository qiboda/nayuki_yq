module;

#include <render_core/render_core.h>
#include <core/macro/macro.h>

module render_core.device;

Device::~Device()
{
    CleanUp();
}

void Device::CreateDevice( const vk::DeviceCreateInfo& deviceInfo )
{
    NY_ASSERT( mPhysicalDevice );

    vk::Device device = mPhysicalDevice->GetRaw().createDevice( deviceInfo );
    mDevice = vk::SharedDevice( device );
}

void Device::DestroyDevice()
{
    mDevice.reset();
}
