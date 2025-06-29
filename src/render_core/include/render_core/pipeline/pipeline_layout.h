#pragma once

#include <core/core.h>
#include <render_core/render_core.h>

class RENDER_CORE_API PipelineLayout : public IRAII
{
  public:
    PipelineLayout();
    virtual ~PipelineLayout() override;

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

  protected:
    vk::SharedDevice mDevice;

    vk::SharedPipelineLayout mPipelineLayout;
};

RENDER_TYPE_WRAPPER( PipelineLayout )