#pragma once

#include "render_core/device/device.h"
#include "render_core/device/physical_device.h"
#include "render_core/instance.h"

class RENDER_CORE_API RenderContext : public Singleton<RenderContext>, public NonCopyable
{
    friend class Singleton<RenderContext>;

  public:
    RenderContext();
    virtual ~RenderContext() override;

  public:
    SharedRenderInstance mRenderInstance;
    SharedPhysicalDevice mPhysicalDevice;
    SharedDevice mDevice;
};