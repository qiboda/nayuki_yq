#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API CommandPool : public IRAII
{
  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

  public:
    void SetDevice( vk::SharedDevice device );
    void SetGraphicsQueueFamilyIndex( u32 index )
    {
        graphicsQueueFamilyIndex = index;
    }

  private:
    void CreateCommandPool();
    void DestroyCommandPool();

  private:
    vk::SharedDevice mDevice;
    vk::SharedCommandPool mCommandPool;

    u32 graphicsQueueFamilyIndex = 0;
};
