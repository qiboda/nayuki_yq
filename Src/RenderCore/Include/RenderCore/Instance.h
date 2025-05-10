#pragma once

#include "Core/Misc/NonCopyable.h"
#include <RenderCore/Window.h>

using VkApiVersion = u32;

class RENDER_CORE_API RenderInstance final : public NonCopyable
{
  public:
    void SetVkApiVersion( VkApiVersion version ) { vk_api_version = version; }

    /// @brief 创建 vulkan instance
    void CreateInstance( vk::ApplicationInfo &appInfo, Window *window );

    vk::Result CreateDebugUtilsMessengerEXT();

    VkResult DestroyDebugUtilsMessengerEXT();

  protected:
    vk::UniqueInstance instance;

    VkApiVersion vk_api_version = VK_API_VERSION_1_4;

#ifdef DEBUG
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;
#endif
};
