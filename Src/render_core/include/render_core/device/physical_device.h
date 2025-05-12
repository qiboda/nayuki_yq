#pragma once

#include "render_core/instance.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API PhysicalDevice : public IRAII
{
  public:
    PhysicalDevice();
    virtual ~PhysicalDevice() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreatePhysicalDevice();

    void DestroyPhysicalDevice();

    void SetInstance( SharedRenderInstance instance )
    {
        mInstance = instance;
    }

    vk::SharedPhysicalDevice &GetShared()
    {
        return mPhysicalDevice;
    }

    vk::PhysicalDevice GetRaw()
    {
        return mPhysicalDevice.get();
    }

  protected:
    SharedRenderInstance mInstance;
    vk::SharedPhysicalDevice mPhysicalDevice;
};

RENDER_TYPE_WRAPPER( PhysicalDevice )