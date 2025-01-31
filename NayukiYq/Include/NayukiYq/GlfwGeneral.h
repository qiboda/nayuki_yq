#pragma once

#ifndef GLFW_GENERAL_H
#define GLFW_GENERAL_H

#include <NayukiYq/NayukiYq.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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

    return true;
}

inline void TerminateWindow() {
    /*待Ch1-4填充*/
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