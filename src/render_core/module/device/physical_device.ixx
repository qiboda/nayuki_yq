module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>

export module render_core.physical_device;

import std;
import core.type;
import core.misc.iraii;
import render_core.render_instance;
import render_core.window;

export class RENDER_CORE_API PhysicalDevice : public IRAII
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

  public:
    std::pair<u32, u32> findGraphicsAndPresentQueueFamilyIndex( Window* window );

  public:
    void SetInstance( SharedRenderInstance instance )
    {
        mInstance = instance;
    }

    vk::SharedPhysicalDevice& GetShared()
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