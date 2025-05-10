#pragma once

#include <render_core/window.h>
#include <core/minimal.h>
#include <render_core/minimal.h>

using VkApiVersion = u32;

class RENDER_CORE_API RenderInstance final : public NonCopyable
{
  public:
    void SetVkApiVersion( VkApiVersion version )
    {
        mVkAapiVersion = version;
    }

    /// @brief 创建 vulkan instance
    void CreateInstance( vk::ApplicationInfo &appInfo, Window *window );

    void CreatePhysicalDevice();

#pragma region DebugUtils
  protected:
    vk::Result CreateDebugUtilsMessengerEXT();
    vk::Result DestroyDebugUtilsMessengerEXT();
#pragma endregion DebugUtils

  protected:
    vk::UniqueInstance mInstance;

    VkApiVersion mVkAapiVersion = VK_API_VERSION_1_4;

#ifdef DEBUG
    vk::UniqueDebugUtilsMessengerEXT mDebugUtilsMessenger;
#endif
};
