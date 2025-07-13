module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>

export module render_core.render_instance;

import core;
import core.misc.iraii;
import render_core.render_window;
import std;

export using VkApiVersion = u32;

export class RENDER_CORE_API RenderInstance final : public IRAII
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
    void CreateInstance( vk::ApplicationInfo& appInfo, std::shared_ptr<IRenderWindow> window );

    void CreatePhysicalDevice();

  public:
    vk::SharedInstance& GetShared()
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

    vk::detail::DynamicLoader dl;

    VkApiVersion mVkAapiVersion = VK_API_VERSION_1_4;

#ifdef DEBUG
    vk::UniqueDebugUtilsMessengerEXT mDebugUtilsMessenger;
#endif
};

RENDER_TYPE_WRAPPER( RenderInstance )