#pragma once

#include "module_export.h"

class RENDER_CORE_API CommandBufferBase : public IRAII
{
  public:
    CommandBufferBase();
    virtual ~CommandBufferBase() override;

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

  public:
    void CreateCommandBuffer();
    void DestroyCommandBuffer();

  public:
    void BeginCommandBuffer( const vk::CommandBufferBeginInfo& beginInfo );
    void EndCommandBuffer();

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    void SetCommandPool( vk::SharedCommandPool commandPool )
    {
        mCommandPool = commandPool;
    }

  private:
    vk::SharedCommandPool mCommandPool;
    vk::SharedDevice mDevice;
    vk::SharedCommandBuffer mCommandBuffer;
};

RENDER_TYPE_WRAPPER( CommandBufferBase )