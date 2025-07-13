module;

// #include "render_core/device/device.h"
// #include "render_core/device/physical_device.h"
// #include "render_core/instance.h"

#include "module_export.h"

export module render_core.render_context;

import core.misc.singleton;
import core.misc.non_copyable;
import render_core.device;
import render_core.physical_device;
import render_core.render_instance;

export class RENDER_CORE_API RenderContext : public Singleton<RenderContext>, public NonCopyable
{
    friend class Singleton<RenderContext>;

  public:
    RenderContext()
    {
    }

    virtual ~RenderContext() override
    {
    }

  public:
    SharedRenderInstance mRenderInstance;
    SharedPhysicalDevice mPhysicalDevice;
    SharedDevice mDevice;
};