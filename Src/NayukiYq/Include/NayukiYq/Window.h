
#pragma once

#include "Core/Misc/Tickable.h"
#include <Core/Core.h>
#include <Core/Misc/RAII.h>
#include <GLFW/glfw3.h>
#include <NayukiYq/NayukiYq.h>
#include <cstddef>

class NAYUKI_YQ_API Window : public IRAII, public ITickable {

  public:
    static void Init() { glfwInit(); }
    static void Terminate() { glfwTerminate(); }

  public:
    Window() {}
    virtual ~Window() {}

  public:
    virtual void Initialize() override {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        NY_ASSERT(mWindow == nullptr, "Window is already initialized.");
        mWindow =
            glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    }

    virtual void CleanUp() override { glfwDestroyWindow(mWindow); }

    virtual void Tick(float deltaTime) override {
        if (mWindow) {
            glfwPollEvents();
        }
    }

    void SetWindowSize(i32 width, i32 height) {
        mWidth = width;
        mHeight = height;
        if (mWindow) {
            glfwSetWindowSize(mWindow, width, height);
        }
    }

    void SetWindowTitle(const std::string &title) {
        mTitle = title;
        if (mWindow) {
            glfwSetWindowTitle(mWindow, title.c_str());
        }
    }

    void CloseWindow() {
        if (mWindow) {
            glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
        }
    }

    bool ShouldClose() { return glfwWindowShouldClose(mWindow) == GLFW_TRUE; }

  protected:
    GLFWwindow *mWindow = nullptr;

    i32 mWidth = 1280;
    i32 mHeight = 720;
    std::string mTitle = "NayukiYq";
};
