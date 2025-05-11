
#include <render_core/minimal.h>
#include "render_core/swapchain.h"
#include "render_core/vktype_conv.h"

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

void Swapchain::SetWindow( Window *window )
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
