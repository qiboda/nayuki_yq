
module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <memory>

export module nayuki.application;

import core;
import core.memory;
import core.misc.iraii;
import render_core.window;
import render_core.render_instance;
import nayuki;

// #include <render_core/window.h>
// #include <render_core/instance.h>

export class NAYUKI_API Application : public IRAII
{
  public:
    Application();

    virtual ~Application() override
    {
    }

    virtual void Initialize() override;

    virtual void CleanUp() override;

    virtual void Update();

  public:
    void SetAppName( const std::string_view& name )
    {
        mAppInfo.setPApplicationName( name.data() );
    }

    void SetAppVersion( u32 major, u32 minor, u32 patch )
    {
        mAppInfo.setApiVersion( VK_MAKE_VERSION( major, minor, patch ) );
    }

    void SetEngineName( const std::string_view& name )
    {
        mAppInfo.setPEngineName( name.data() );
    }

    void SetEngineVersion( u32 major, u32 minor, u32 patch )
    {
        mAppInfo.setApiVersion( VK_MAKE_VERSION( major, minor, patch ) );
    }

  protected:
    void initWindow();

  protected:
    std::shared_ptr<Window> mWindow = nullptr;

    SharedRenderInstance mRenderInstance = nullptr;

    vk::ApplicationInfo mAppInfo = vk::ApplicationInfo(
        "Hello World!", VK_MAKE_VERSION( 0, 1, 0 ), "No Engine", VK_MAKE_VERSION( 0, 1, 0 ), VK_API_VERSION_1_0 );
};