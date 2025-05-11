#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

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

