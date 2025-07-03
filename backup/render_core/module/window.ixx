module;

#include <render_core/render_core.h>
// #include "core/math/math.h"
import core.math;
// #include "core/misc/tickable.h"
import core.misc;
import core;

export module render_core.window;

export class RENDER_CORE_API Window : public IRAII, public ITickable
{

  public:
    static void Init()
    {
        glfwInit();
    }

    static void Terminate()
    {
        glfwTerminate();
    }

  public:
    Window();

    virtual ~Window() override
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

    virtual void Tick( float deltaTime ) override
    {
        UNUSED_VARS( deltaTime );
        if ( mWindow )
        {
            glfwPollEvents();
        }
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
    void CreateSurface( std::shared_ptr<class RenderInstance> instance );

    std::vector<const char*> GetRenderInstanceExtensions();

  protected:
    GLFWwindow* mWindow = nullptr;

    vk::SharedSurfaceKHR mSharedSurfaceKHR;

    Extent2D mExtent2D = { 1280, 720 };
    std::string mTitle = "NayukiYq";
};
