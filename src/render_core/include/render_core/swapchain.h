#pragma once

#include "module_export.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

#include <render_core/instance.h>
#include <render_core/swapchain.h>
#include <render_core/window.h>

#include <render_core/command/command_pool.h>
#include <render_core/command/command_buffer.h>

class RENDER_CORE_API Swapchain : public IRAII
{
  public:
    Swapchain();
    virtual ~Swapchain() override;

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

  public:
    void SetWindow( Window* window );
    void SetDevice( vk::SharedDevice device );
    void SetCommandPool( vk::SharedCommandPool commandPool );

  private:
    void CreateSwapchain();
    void DestroySwapchain();

  private:
    Window* mWindow = nullptr;
    vk::SharedSwapchainKHR mSwapchain;
    vk::SharedDevice mDevice;
    vk::SharedCommandPool mCommandPool;
};

RENDER_TYPE_WRAPPER( Swapchain )