
#pragma once

#include <GLFW/glfw3.h>
#include <NayukiYq/NayukiYq.h>
#include <NayukiYq/Window.h>
#include <cstddef>

class NAYUKI_YQ_API Application : public IRAII {

  public:
    Application() {}
    virtual ~Application() {}

    virtual void Initialize() override { initWindow(); }

    virtual void CleanUp() override {
        if (mWindow) {
            mWindow->CleanUp();
            NY_DELETE(mWindow);
        }

        Window::Terminate();
    }

    virtual void Update() {
        while (true) {
            if (mWindow) {
                mWindow->Tick(1.0);
                if (mWindow->ShouldClose()) {
                    break;
                }
            }
        }
    }

  protected:
    void initWindow() {
        Window::Init();

        if (mWindow == nullptr) {
            mWindow = new Window();
            mWindow->SetWindowSize(1280, 720);
            mWindow->Initialize();
        }
    }

  protected:
    Window *mWindow = nullptr;
};