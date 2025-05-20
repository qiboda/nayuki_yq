#include "core/macro/macro.h"
#include "render_core/instance.h"
#include <memory>
#include <render_core/render_core.h>

#include <core/misc/raii.h>
#include <core/misc/non_copyable.h>

#include <render_core/window.h>

Window::Window()
{
}

void Window::CreateSurface( std::shared_ptr<RenderInstance> instance )
{
    NY_ASSERT_MSG( mWindow, "Window is not initialized." )
    NY_ASSERT_MSG( instance->GetRaw(), "Vulkan instance is not initialized." )
    NY_ASSERT_MSG( glfwVulkanSupported(), "Vulkan is not supported on glfw." )
    NY_ASSERT_MSG( !mSharedSurfaceKHR, "Vulkan surface is already initialized." )

    // Create a Vulkan surface using GLFW
    VkSurfaceKHR vkSurface;
    VkResult result = glfwCreateWindowSurface( instance->GetRaw(), mWindow, nullptr, &vkSurface );
    if ( result != VK_SUCCESS )
    {
        NY_LOG_CRITICAL( LogRenderCore, "Failed to create window surface: {}", vk::to_string( vk::Result( result ) ) );
    }
    mSharedSurfaceKHR = vk::SharedSurfaceKHR( vkSurface, instance->GetShared() );
}

std::vector<const char *> Window::GetRenderInstanceExtensions()
{
    u32 glfwExtensionCount = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

    std::vector<const char *> extensionsVec;
    for ( u32 i = 0; i < glfwExtensionCount; i++ )
    {
        SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN
        extensionsVec.push_back( extensions[i] );
        SUPPRESS_UNSAFE_BUFFER_USAGE_END
    }
    return extensionsVec;
}
