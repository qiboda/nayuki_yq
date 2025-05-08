#include "Core/Macro/Macro.h"
#include <RenderCore/RenderCore.h>
#include <RenderCore/Instance.h>
#include <vector>

static PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
static PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pMessenger) {
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator,
                                             pMessenger);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT messenger,
    VkAllocationCallbacks const *pAllocator) {
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugMessageFunc(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                 vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
                 vk::DebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                 void *_pUserData) {
    UNUSED_VAR(_pUserData);

    std::string message;

    message += vk::to_string(messageSeverity) + ": " +
               vk::to_string(messageTypes) + ":\n";
    message += std::string("\t") + "messageIDName   = <" +
               pCallbackData->pMessageIdName + ">\n";
    message += std::string("\t") + "messageIdNumber = " +
               std::to_string(pCallbackData->messageIdNumber) + "\n";
    message += std::string("\t") + "message         = <" +
               pCallbackData->pMessage + ">\n";
    if (0 < pCallbackData->queueLabelCount) {
        message += std::string("\t") + "Queue Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
            message += std::string("\t\t") + "labelName = <" +
                       pCallbackData->pQueueLabels[i].pLabelName + ">\n";
#pragma clang diagnostic pop
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        message += std::string("\t") + "CommandBuffer Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
            message += std::string("\t\t") + "labelName = <" +
                       pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
#pragma clang diagnostic pop
        }
    }
    if (0 < pCallbackData->objectCount) {
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
            message += std::string("\t") + "Object " + std::to_string(i) + "\n";
            message += std::string("\t\t") + "objectType   = " +
                       vk::to_string(pCallbackData->pObjects[i].objectType) +
                       "\n";
            message += std::string("\t\t") + "objectHandle = " +
                       std::to_string(pCallbackData->pObjects[i].objectHandle) +
                       "\n";
            if (pCallbackData->pObjects[i].pObjectName) {
                message += std::string("\t\t") + "objectName   = <" +
                           pCallbackData->pObjects[i].pObjectName + ">\n";
#pragma clang diagnostic pop
            }
        }
    }

    NY_LOG_WARN(LogRenderCore, "Vulkan debug message: {}", message);

    return false;
}

void RenderInstance::CreateInstance(vk::ApplicationInfo &appInfo,
                                    Window *window) {
    NY_LOG_INFO(LogRenderCore, "Creating Render Instance.");

    NY_CHECK(!this->instance)

    appInfo.setApiVersion(vk_api_version);

    std::vector<const char *> renderInstanceExtensions =
        window->GetRenderInstanceExtensions();

    std::vector<const char *> renderInstanceLayers;
    renderInstanceLayers.push_back("VK_LAYER_KHRONOS_validation");

    // #if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    //     // initialize the DipatchLoaderDynamic to use
    //     VULKAN_HPP_DEFAULT_DISPATCHER.init();
    // #endif

#ifdef DEBUG
    renderInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    renderInstanceExtensions.push_back(
        VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);

#endif

    auto createInfo =
        vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo,
                               renderInstanceLayers,    // enabled layers
                               renderInstanceExtensions // enabled extensions
        );

    try {
        instance = vk::createInstanceUnique(createInfo, nullptr);
    } catch (vk::SystemError err) {
        NY_LOG_ERROR(LogRenderCore, "create isntance error", err.what());
        // throw std::runtime_error(std::string("failed to create instance!") +
        //                          err.what());
    }

    NY_LOG_INFO(LogRenderCore, "Vulkan instance available extensions:");
    for (const auto &extension : vk::enumerateInstanceExtensionProperties()) {
        NY_LOG_INFO(LogRenderCore, "\t {}", extension.extensionName.data());
    }

    // #if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    //     // initialize the DipatchLoaderDynamic to use
    //     VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
    // #endif

    CreateDebugUtilsMessengerEXT();
}

vk::Result RenderInstance::CreateDebugUtilsMessengerEXT() {

#ifdef DEBUG
    NY_LOG_INFO(LogRenderCore, "Creating debug utils messenger.");

    NY_CHECK(!debugUtilsMessenger)

#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wcast-function-type"
    pfnVkCreateDebugUtilsMessengerEXT =
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            instance->getProcAddr("vkCreateDebugUtilsMessengerEXT"));
    if (!pfnVkCreateDebugUtilsMessengerEXT) {
        std::cout << "GetInstanceProcAddr: Unable to find "
                     "pfnVkCreateDebugUtilsMessengerEXT function."
                  << std::endl;
        exit(1);
    }

    pfnVkDestroyDebugUtilsMessengerEXT =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance->getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    if (!pfnVkDestroyDebugUtilsMessengerEXT) {
        std::cout << "GetInstanceProcAddr: Unable to find "
                     "pfnVkDestroyDebugUtilsMessengerEXT function."
                  << std::endl;
        exit(1);
    }
#    pragma clang diagnostic pop

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.setMessageSeverity(severityFlags);
    createInfo.setMessageType(messageTypeFlags);
    createInfo.pfnUserCallback = debugMessageFunc;

    debugUtilsMessenger =
        instance->createDebugUtilsMessengerEXTUnique(createInfo);

    NY_CHECK(debugUtilsMessenger)
#endif

    return vk::Result::eSuccess;
}
