module;

#include <render_core/render_core.h>
#include <core/macro/macro.h>
#include <core/compiler/diagnostic.h>
#include <core/logger/log.h>

module render_core.render_instance;

import std;
import render_core;
import render_core.render_window;
import core.logger.logger;

#ifndef NDEBUG
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

void RenderInstance::CreateInstance( vk::ApplicationInfo& appInfo, std::shared_ptr<IRenderWindow> window )
{
    NY_LOG_INFO( LogRenderCore, "Creating Render Instance." );

    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    VULKAN_HPP_DEFAULT_DISPATCHER.init( dl );

    // 初始化步骤
    auto getInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( getInstanceProcAddr );

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
        VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );
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
    mInstance->destroyDebugUtilsMessengerEXT( mDebugUtilsMessenger.get(), nullptr, VULKAN_HPP_DEFAULT_DISPATCHER );
    mDebugUtilsMessenger.reset();
#endif
    return vk::Result::eSuccess;
}

vk::Result RenderInstance::CreateDebugUtilsMessengerEXT()
{
#ifndef NDEBUG
    NY_LOG_INFO( LogRenderCore, "Creating debug utils messenger." );

    NY_ASSERT( !mDebugUtilsMessenger )

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.setMessageSeverity( severityFlags );
    createInfo.setMessageType( messageTypeFlags );
    createInfo.pfnUserCallback = debugMessageFunc;

    mDebugUtilsMessenger =
        mInstance->createDebugUtilsMessengerEXTUnique( createInfo, nullptr, VULKAN_HPP_DEFAULT_DISPATCHER );

    NY_ASSERT( mDebugUtilsMessenger )
#endif

    return vk::Result::eSuccess;
}

void RenderInstance::CreatePhysicalDevice()
{
}
