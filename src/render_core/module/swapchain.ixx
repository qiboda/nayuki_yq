module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>

export module render_core.swapchain;

import std;
import core.misc.iraii;
import render_core.window;
import render_core.vkconv;
import render_core.command_pool;
import render_core.command_buffer;
import render_core.device;
import render_core.render_instance;

export class RENDER_CORE_API Swapchain : public IRAII
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