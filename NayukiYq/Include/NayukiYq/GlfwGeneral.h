#pragma once

#ifndef GLFW_GENERAL_H
#define GLFW_GENERAL_H

#include <NayukiYq/NayukiYq.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <NayukiYq/GraphicsBase.h>
#include <flecs.h>

struct Window {
    // 窗口的指针，全局变量自动初始化为NULL
    GLFWwindow *pWindow = nullptr;
    // 显示器信息的指针
    GLFWmonitor *pMonitor = nullptr;
};

struct WindowConfig {
    // 窗口标题
    const char *windowTitle = "EasyVK";
    VkExtent2D windowSize = {1280, 720};
    VkOffset2D windowPosition = {0, 0};
    bool initFullScreen = false;
    bool isResizable = true;
    bool limitFrameRate = true;
};

inline bool InitializeWindow(flecs::iter &it) {
    Window *window = it.world().get_mut<Window>();
    const WindowConfig *config = it.world().get<WindowConfig>();

    std::cout << "InitializeWindow" << std::endl;

    if (!glfwInit()) {
        NY_LOG_ERROR(
            LogNayukiYq,
            "[ InitializeWindow ] ERROR\nFailed to initialize GLFW!\n");
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, config->isResizable);

    window->pMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *pMode = glfwGetVideoMode(window->pMonitor);
    window->pWindow =
        config->initFullScreen
            ? glfwCreateWindow(pMode->width, pMode->height, config->windowTitle,
                               window->pMonitor, nullptr)
            : glfwCreateWindow(config->windowSize.width,
                               config->windowSize.height, config->windowTitle,
                               nullptr, nullptr);

    if (!window->pWindow) {
        NY_LOG_ERROR(LogNayukiYq,
                     "[ InitializeWindow ]\nFailed to create a glfw window!\n");
        glfwTerminate();
        return false;
    }

    uint32_t extensionCount = 0;
    const char **extensionNames =
        glfwGetRequiredInstanceExtensions(&extensionCount);
    if (!extensionNames) {
        NY_LOG_ERROR(
            LogNayukiYq,
            "[ InitializeWindow ]\nVulkan is not available on this machine!\n");
        glfwTerminate();
        return false;
    }

    for (size_t i = 0; i < extensionCount; i++) {
        GraphicsBase::Get().AddInstanceExtension(extensionNames[i]);
        // graphicsBase::Base().AddInstanceExtension("VK_KHR_surface");
        // graphicsBase::Base().AddInstanceExtension("VK_KHR_win32_surface");
    }

    GraphicsBase::Get().AddDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // 在创建window surface前创建Vulkan实例
    graphicsBase::Base()
        .UseLatestApiVersion() if (graphicsBase::Base()
                                       .CreateInstance()) return false;

    // 创建window surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (VkResult result = glfwCreateWindowSurface(
            graphicsBase::Base().Instance(), pWindow, nullptr, &surface)) {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to create "
                                 "a window surface!\nError code: {}\n",
                                 int32_t(result));
        glfwTerminate();
        return false;
    }
    graphicsBase::Base().Surface(surface);

    // 通过用||操作符短路执行来省去几行
    if ( // 获取物理设备，并使用列表中的第一个物理设备，这里不考虑以下任意函数失败后更换物理设备的情况
        graphicsBase::Base().GetPhysicalDevices() ||
        // 一个true一个false，暂时不需要计算用的队列
        graphicsBase::Base().DeterminePhysicalDevice(0, true, false) ||
        // 创建逻辑设备
        graphicsBase::Base().CreateDevice())
        return false;
    //----------------------------------------

    if (GraphicsBase::Base().CreateSwapchain(limitFrameRate))
        return false;

    return true;
}

inline void TerminateWindow() {
    GraphicsBase::Base().WaitIdle()
    glfwTerminate();
}

inline void TitleFps(flecs::iter &it) {
    std::cout << "TitleFps" << std::endl;

    Window *window = it.world().get_mut<Window>();
    const WindowConfig *config = it.world().get<WindowConfig>();

    static double time0 = glfwGetTime();
    static double time1;
    static double dt;
    static int dframe = -1;
    static std::stringstream info;
    time1 = glfwGetTime();
    dframe++;
    if ((dt = time1 - time0) >= 1) {
        info.precision(1);
        info << config->windowTitle << "    " << std::fixed << dframe / dt
             << " FPS";
        glfwSetWindowTitle(window->pWindow, info.str().c_str());
        info.str(""); // 别忘了在设置完窗口标题后清空所用的stringstream
        time0 = time1;
        dframe = 0;
    }
}

void MakeWindowFullScreen(Window &window) {
    const GLFWvidmode *pMode = glfwGetVideoMode(window.pMonitor);
    glfwSetWindowMonitor(window.pWindow, window.pMonitor, 0, 0, pMode->width,
                         pMode->height, pMode->refreshRate);
}

void MakeWindowWindowed(Window &window, const WindowConfig &config) {
    // VkOffset2D position, VkExtent2D size
    const GLFWvidmode *pMode = glfwGetVideoMode(window.pMonitor);
    glfwSetWindowMonitor(window.pWindow, nullptr, config.windowPosition.x,
                         config.windowPosition.y, config.windowSize.width,
                         config.windowSize.height, pMode->refreshRate);
}

struct NAYUKIYQ_API WindowModule {

    // fix: clang-format error, 增加一行，就正确了。。。。。
    using Window = Window;

    /// 构建
    inline explicit WindowModule(flecs::world &world) {
        world.module<WindowModule>("WindowModule");

        world.component<Window>();
        world.component<WindowConfig>();

        world.set<Window>({nullptr, nullptr});
        world.set<WindowConfig>({
            "EasyVK",
            {1280, 720},
            {0, 0},
            false,
            true,
            true,
        });

        world.system<>("InitializeWindow")
            .kind(flecs::OnStart)
            .run(InitializeWindow);

        world.system<>("TitleFps").kind(flecs::OnUpdate).run(TitleFps);
    }
};

#endif // !GLFW_GENERAL_H