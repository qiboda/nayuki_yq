module;

#include <memory>

module nayuki.application;

// import std;
import nayuki;

Application::Application()
{
}

void Application::Initialize()
{
    initWindow();
    if ( mRenderInstance == nullptr )
    {
        mRenderInstance = std::make_shared<RenderInstance>();
        mRenderInstance->Initialize();
        mRenderInstance->CreateInstance( mAppInfo, mWindow );

        mWindow->CreateSurface( mRenderInstance );
    }
}

void Application::CleanUp()
{
    if ( mRenderInstance )
    {
        mRenderInstance->CleanUp();
        mRenderInstance.reset();
    }

    if ( mWindow )
    {
        mWindow->CleanUp();
        mWindow.reset();
    }

    Window::Terminate();
}

void Application::Update()
{
    while ( true )
    {
        if ( mWindow )
        {
            mWindow->Tick( 1.0 );
            if ( mWindow->ShouldClose() )
            {
                break;
            }
        }
    }
}

void Application::initWindow()
{
    Window::Init();

    if ( mWindow == nullptr )
    {
        mWindow = std::make_shared<Window>();
        mWindow->SetWindowSize( 1280, 720 );
        mWindow->Initialize();
    }
}
