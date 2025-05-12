#pragma once

#include <memory>
#include <render_core/window.h>
#include <core/minimal.h>
#include <render_core/minimal.h>

using VkApiVersion = u32;

class RENDER_CORE_API RenderInstance final : public IRAII
{
  public:
    void SetVkApiVersion( VkApiVersion version )
    {
        mVkAapiVersion = version;
    }

    virtual void Initialize() override
    {
        // CreateInstance( mAppInfo, mWindow );
    }

    virtual void CleanUp() override
    {
        DestroyDebugUtilsMessengerEXT();
        mInstance.reset();
    }

    /// @brief 创建 vulkan instance
    void CreateInstance( vk::ApplicationInfo &appInfo, std::shared_ptr<Window> window );

    void CreatePhysicalDevice();

  public:
    vk::SharedInstance &GetShared()
    {
        return mInstance;
    }

    vk::Instance GetRaw()
    {
        return mInstance.get();
    }

#pragma region DebugUtils
  protected:
    vk::Result CreateDebugUtilsMessengerEXT();
    vk::Result DestroyDebugUtilsMessengerEXT();
#pragma endregion DebugUtils

  protected:
    vk::SharedInstance mInstance;

    VkApiVersion mVkAapiVersion = VK_API_VERSION_1_4;

#ifdef DEBUG
    vk::UniqueDebugUtilsMessengerEXT mDebugUtilsMessenger;
#endif
};

RENDER_TYPE_WRAPPER( RenderInstance )