module;

// #include "render_core/device/physical_device.h"

export module render_core.device;

import render_core.physical_device;

export class RENDER_CORE_API Device : public IRAII
{
  public:
    Device()
    {
    }

    virtual ~Device() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreateDevice( const vk::DeviceCreateInfo& deviceInfo );

    void DestroyDevice();

  public:
    void SetPhysicalDevice( SharedPhysicalDevice physicalDevice )
    {
        mPhysicalDevice = physicalDevice;
    }

    vk::SharedDevice& GetShared()
    {
        return mDevice;
    }

    vk::Device GetRaw()
    {
        return mDevice.get();
    }

  protected:
    SharedPhysicalDevice mPhysicalDevice;
    vk::SharedDevice mDevice;
    vk::DeviceCreateInfo mDeviceCreateInfo;
};

Device::~Device()
{
    CleanUp();
}

void Device::CreateDevice( const vk::DeviceCreateInfo& deviceInfo )
{
    NY_ASSERT( mPhysicalDevice )

    vk::Device device = mPhysicalDevice->GetRaw().createDevice( deviceInfo );
    mDevice = vk::SharedDevice( device );
}

void Device::DestroyDevice()
{
    mDevice.reset();
}

RENDER_TYPE_WRAPPER( Device )