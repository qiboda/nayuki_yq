module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>

export module render_core.device;

import std;
import render_core.physical_device;
import core.misc.iraii;

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

RENDER_TYPE_WRAPPER( Device )