#pragma once

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

RENDER_TYPE_WRAPPER( CommandPool )