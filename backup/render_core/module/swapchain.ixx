module;

#include "module_export.h"

// #include <render_core/instance.h>
// #include <render_core/swapchain.h>
// #include <render_core/window.h>
// #include "render_core/vktype_conv.h"

// #include <render_core/command/command_pool.h>
// #include <render_core/command/command_buffer.h>

export module render_core.swapchain;

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

Swapchain::Swapchain()
{
}

Swapchain::~Swapchain()
{
}

void Swapchain::Initialize()
{
    CreateSwapchain();
}

void Swapchain::CleanUp()
{
    DestroySwapchain();
}

void Swapchain::SetWindow( Window* window )
{
    mWindow = window;
}

void Swapchain::SetDevice( vk::SharedDevice device )
{
    mDevice = device;
}

void Swapchain::SetCommandPool( vk::SharedCommandPool commandPool )
{
    mCommandPool = commandPool;
}

void Swapchain::CreateSwapchain()
{
    // Create swapchain
    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.setSurface( mWindow->GetSurface().get() );
    createInfo.setMinImageCount( 3 );
    // TODO 应该查询屏幕，获取显示器的颜色格式。
    createInfo.setImageFormat( vk::Format::eB8G8R8A8Srgb );
    // TODO 应该查询屏幕，获取显示器的颜色空间。
    createInfo.setImageColorSpace( vk::ColorSpaceKHR::eSrgbNonlinear );
    createInfo.setImageExtent( vkconv::From( mWindow->GetWindowSize() ) );
    createInfo.setImageArrayLayers( 1 );
    createInfo.setImageUsage( vk::ImageUsageFlagBits::eColorAttachment );

    auto Swapchain = mDevice->createSwapchainKHR( createInfo );
    mSwapchain = vk::SharedSwapchainKHR( Swapchain, mDevice, mWindow->GetSurface() );
}

void Swapchain::DestroySwapchain()
{
    if ( mSwapchain )
    {
        mDevice->destroySwapchainKHR( mSwapchain.get() );
        mSwapchain.reset();
    }
}

RENDER_TYPE_WRAPPER( Swapchain )