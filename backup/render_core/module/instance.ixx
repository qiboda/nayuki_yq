module;

// #include <memory>
#include <render_core/render_core.h>

export module render_core.render_instance;

import render_core.window;
import std;

using VkApiVersion = u32;

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
    void CreateInstance( vk::ApplicationInfo& appInfo, std::shared_ptr<Window> window );

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

    VkApiVersion mVkAapiVersion = VK_API_VERSION_1_4;

#ifdef DEBUG
    vk::UniqueDebugUtilsMessengerEXT mDebugUtilsMessenger;
#endif
};

#ifdef DEBUG
/// 保存函数地址。
static PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
static PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance instance,
                                                               const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                               const VkAllocationCallbacks* pAllocator,
                                                               VkDebugUtilsMessengerEXT* pMessenger )
{
    return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance instance,
                                                            VkDebugUtilsMessengerEXT messenger,
                                                            VkAllocationCallbacks const* pAllocator )
{
    return pfnVkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
}

static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugMessageFunc( vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
                                                          vk::DebugUtilsMessengerCallbackDataEXT const* pCallbackData,
                                                          void* _pUserData )
{
    UNUSED_VARS( _pUserData );

    std::string message;

    message += vk::to_string( messageSeverity ) + ": " + vk::to_string( messageTypes ) + ":\n";
    message += std::string( "\t" ) + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
    message += std::string( "\t" ) + "messageIdNumber = " + std::to_string( pCallbackData->messageIdNumber ) + "\n";
    message += std::string( "\t" ) + "message         = <" + pCallbackData->pMessage + ">\n";
    if ( 0 < pCallbackData->queueLabelCount )
    {
        message += std::string( "\t" ) + "Queue Labels:\n";
        for ( uint32_t i = 0; i < pCallbackData->queueLabelCount; i++ )
        {
            SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
            SUPPRESS_UNSAFE_BUFFER_USAGE_END
        }
    }
    if ( 0 < pCallbackData->cmdBufLabelCount )
    {
        message += std::string( "\t" ) + "CommandBuffer Labels:\n";
        for ( uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ )
        {
            SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
            SUPPRESS_UNSAFE_BUFFER_USAGE_END
        }
    }
    if ( 0 < pCallbackData->objectCount )
    {
        for ( uint32_t i = 0; i < pCallbackData->objectCount; i++ )
        {
            SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN
            message += std::string( "\t" ) + "Object " + std::to_string( i ) + "\n";
            message += std::string( "\t\t" ) +
                       "objectType   = " + vk::to_string( pCallbackData->pObjects[i].objectType ) + "\n";
            message += std::string( "\t\t" ) +
                       "objectHandle = " + std::to_string( pCallbackData->pObjects[i].objectHandle ) + "\n";
            if ( pCallbackData->pObjects[i].pObjectName )
            {
                message += std::string( "\t\t" ) + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
            }
            SUPPRESS_UNSAFE_BUFFER_USAGE_END
        }
    }

    NY_LOG_WARN( LogRenderCore, "Vulkan debug message: {}", message );

    return false;
}
#endif // DEBUG

void RenderInstance::CreateInstance( vk::ApplicationInfo& appInfo, std::shared_ptr<Window> window )
{
    NY_LOG_INFO( LogRenderCore, "Creating Render Instance." );

    NY_ASSERT( !this->mInstance )

    appInfo.setApiVersion( mVkAapiVersion );

    std::vector<const char*> renderInstanceExtensions = window->GetRenderInstanceExtensions();

    std::vector<const char*> renderInstanceLayers;
    renderInstanceLayers.push_back( "VK_LAYER_KHRONOS_validation" );

    // #if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    //     // initialize the DispatchLoaderDynamic to use
    //     VULKAN_HPP_DEFAULT_DISPATCHER.init();
    // #endif

#ifdef DEBUG
    renderInstanceExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
    renderInstanceExtensions.push_back( VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME );
#endif

    auto createInfo = vk::InstanceCreateInfo( vk::InstanceCreateFlags(),
                                              &appInfo,
                                              renderInstanceLayers,    // enabled layers
                                              renderInstanceExtensions // enabled extensions
    );

    try
    {
        auto instance = vk::createInstance( createInfo, nullptr );
        mInstance = vk::SharedInstance( instance );
    }
    catch ( vk::SystemError err )
    {
        NY_LOG_CRITICAL( LogRenderCore, "create instance error: ", err.what() );
    }

    NY_LOG_INFO( LogRenderCore, "Vulkan instance available extensions:" );
    for ( const auto& extension : vk::enumerateInstanceExtensionProperties() )
    {
        NY_LOG_INFO( LogRenderCore, "\t {}", extension.extensionName.data() );
    }

    // #if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    //     // initialize the DispatchLoaderDynamic to use
    //     VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
    // #endif

    CreateDebugUtilsMessengerEXT();
}

vk::Result RenderInstance::DestroyDebugUtilsMessengerEXT()
{
#ifdef DEBUG
    mInstance->destroyDebugUtilsMessengerEXT( mDebugUtilsMessenger.get() );
    mDebugUtilsMessenger.reset();
#endif
    return vk::Result::eSuccess;
}

vk::Result RenderInstance::CreateDebugUtilsMessengerEXT()
{
#ifdef DEBUG
    NY_LOG_INFO( LogRenderCore, "Creating debug utils messenger." );

    NY_ASSERT( !mDebugUtilsMessenger )

    SUPPRESS_CAST_FUNCTION_TYPE_BEGIN
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        mInstance->getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
    if ( !pfnVkCreateDebugUtilsMessengerEXT )
    {
        NY_LOG_CRITICAL( LogRenderCore,
                         "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." );
    }

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        mInstance->getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
    if ( !pfnVkDestroyDebugUtilsMessengerEXT )
    {
        NY_LOG_CRITICAL( LogRenderCore,
                         "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." );
    }

    SUPPRESS_CAST_FUNCTION_TYPE_END

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.setMessageSeverity( severityFlags );
    createInfo.setMessageType( messageTypeFlags );
    createInfo.pfnUserCallback = debugMessageFunc;

    mDebugUtilsMessenger = mInstance->createDebugUtilsMessengerEXTUnique( createInfo );

    NY_ASSERT( mDebugUtilsMessenger )
#endif

    return vk::Result::eSuccess;
}

void RenderInstance::CreatePhysicalDevice()
{
}

RENDER_TYPE_WRAPPER( RenderInstance )