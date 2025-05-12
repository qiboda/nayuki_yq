
#pragma once

#include <memory>
#include <nayuki_yq/nayuki_yq.h>
#include <render_core/window.h>
#include <render_core/instance.h>

class NAYUKI_YQ_API Application : public IRAII
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
    void SetAppName( const std::string_view &name )
    {
        mAppInfo.setPApplicationName( name.data() );
    }

    void SetAppVersion( u32 major, u32 minor, u32 patch )
    {
        mAppInfo.setApiVersion( VK_MAKE_VERSION( major, minor, patch ) );
    }

    void SetEngineName( const std::string_view &name )
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

    std::shared_ptr<RenderInstance> mRenderInstance = nullptr;

    vk::ApplicationInfo mAppInfo = vk::ApplicationInfo(
        "Hello World!", VK_MAKE_VERSION( 0, 1, 0 ), "No Engine", VK_MAKE_VERSION( 0, 1, 0 ), VK_API_VERSION_1_0 );
};