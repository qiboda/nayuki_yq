#include <RenderCore/RenderCore.h>

#include <RenderCore/Instance.h>

void RenderInstance::CreateInstance(vk::ApplicationInfo &appInfo,
                                    Window *window) {
    appInfo.setApiVersion(vk_api_version);

    u32 renderInstanceExtensionCount = 0;
    const char **renderInstanceExtensions =
        window->GetRenderInstanceExtensions(&renderInstanceExtensionCount);

    auto createInfo = vk::InstanceCreateInfo(
        vk::InstanceCreateFlags(), &appInfo, 0, nullptr, // enabled
        renderInstanceExtensionCount,
        renderInstanceExtensions // enabled extensions
    );

    try {
        instance = vk::createInstanceUnique(createInfo, nullptr);
    } catch (vk::SystemError err) {
        throw std::runtime_error("failed to create instance!");
    }

    NY_LOG_INFO(LogRenderCore, "Vulkan instance available extensions:");
    for (const auto &extension : vk::enumerateInstanceExtensionProperties()) {
        NY_LOG_INFO(LogRenderCore, "\t {}", extension.extensionName.data());
    }
}
