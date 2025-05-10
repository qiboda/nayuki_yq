
#include "render_core/swapchain.h"

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
    // createInfo.setSurface( mWindow->() );
    // createInfo.setMinImageCount( 2 );
    // createInfo.setImageFormat( vk::Format::eB8G8R8A8Srgb );
    // createInfo.setImageColorSpace( vk::ColorSpaceKHR::eSrgbNonlinear );
    // createInfo.setImageExtent( mWindow->GetExtent() );
    // createInfo.setImageArrayLayers( 1 );
    // createInfo.setImageUsage( vk::ImageUsageFlagBits::eColorAttachment );

    // mSwapchain = mDevice->createSwapchainKHR( createInfo );
}

void Swapchain::DestroySwapchain()
{
    if ( mSwapchain )
    {
        // mDevice->destroySwapchainKHR( mSwapchain );
        // mSwapchain = nullptr;
    }
}
