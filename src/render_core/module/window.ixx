module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <core/macro/macro.h>

export module render_core.window;

import std;
import core.math;
import core.misc.tickable;
import core.misc.iraii;
import render_core.render_instance;
import render_core.render_window;
import core;
import ecs;

export struct RENDER_CORE_API Window : public Component
{
    RawWindow* mWindow = nullptr;

    Window()
    {
        mWindow = new RawWindow();
        mWindow->Initialize();
    }

    ~Window() override
    {
        if ( mWindow )
        {
            mWindow->CleanUp();
            delete mWindow;
            mWindow = nullptr;
        }
    }
};

void StartupWindow( Commands& commands )
{
    // clang-format off
    commands()
        ->Entity()
            ->Create()
                ->AddComponent(std::move( Window() ) );
    // clang-format on

    glfwInit();
}

void UpdateWindow( Query<&Window> query )
{
    for ( auto& [window] : query )
    {
        if ( window->mWindow && window->mWindow->ShouldClose() )
        {
            window->mWindow->CloseWindow();
            // todo: 删除window，同时，发送通知事件。
        }
    }
    glfwPollEvents();
}

struct WindowFeature : public IFeature
{
    virtual void Build( Registry* registry ) override
    {
        registry->AddSystem<StartupPhase>( ScheduleSystemNodeConfig::Create( &StartupWindow ).Build() );
        registry->AddSystem<FirstPhase>( ScheduleSystemNodeConfig::Create( &UpdateWindow ).Build() );
    }
};

export class RENDER_CORE_API RawWindow : public IRenderWindow, public IRAII, public ITickable
{
  public:
    static void Init()
    {
    }

    static void Terminate()
    {
        // todo: 需要在所有window删除后，再调用
        glfwTerminate();
    }

  public:
    RawWindow();

    virtual ~RawWindow() override
    {
        CleanUp();
    }

  public:
    virtual void Initialize() override
    {
        NY_ASSERT_MSG( glfwVulkanSupported(), "Vulkan is not supported on glfw." )

        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
        glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

        NY_ASSERT_MSG( mWindow == nullptr, "Window is already initialized." )
        mWindow = glfwCreateWindow( static_cast<i32>( mExtent2D.width ),
                                    static_cast<i32>( mExtent2D.height ),
                                    mTitle.c_str(),
                                    nullptr,
                                    nullptr );
    }

    virtual void CleanUp() override
    {
        glfwDestroyWindow( mWindow );
        mWindow = nullptr;
    }

  public:
    void SetWindowSize( u32 width, u32 height )
    {
        mExtent2D.width = width;
        mExtent2D.height = height;
        if ( mWindow )
        {
            glfwSetWindowSize( mWindow, static_cast<i32>( width ), static_cast<i32>( height ) );
        }
    }

    void SetWindowTitle( const std::string& title )
    {
        mTitle = title;
        if ( mWindow )
        {
            glfwSetWindowTitle( mWindow, title.c_str() );
        }
    }

    vk::SharedSurfaceKHR& GetSurface()
    {
        return mSharedSurfaceKHR;
    }

    /// get window size
    Extent2D GetWindowSize() const
    {
        return mExtent2D;
    }

  public:
    void CloseWindow()
    {
        if ( mWindow )
        {
            glfwSetWindowShouldClose( mWindow, GLFW_TRUE );
        }
    }

    bool ShouldClose()
    {
        return glfwWindowShouldClose( mWindow ) == GLFW_TRUE;
    }

  public:
    void CreateSurface( std::shared_ptr<RenderInstance> instance );

    virtual std::vector<const char*> GetRenderInstanceExtensions() override;

  protected:
    GLFWwindow* mWindow = nullptr;

    vk::SharedSurfaceKHR mSharedSurfaceKHR;

    Extent2D mExtent2D = { 1280, 720 };
    std::string mTitle = "NayukiYq";
};
