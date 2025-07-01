module;

#include <core/core.h>
#include <render_core/render_core.h>

export module render_core.shader;

export class RENDER_CORE_API Shader : public IRAII
{
  public:
    Shader()
    {
    }

    virtual ~Shader() override
    {
    }

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreateShader( const vk::ShaderModuleCreateInfo& shaderInfo )
    {
        NY_ASSERT( mDevice )

        vk::ShaderModule shaderModule = mDevice->createShaderModule( shaderInfo );
        mShaderModule = vk::SharedShaderModule( shaderModule, mDevice );
    }

    void DestroyShader()
    {
        mDevice->destroyShaderModule( mShaderModule.get() );
        mShaderModule.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::ShaderModule GetRaw()
    {
        return mShaderModule.get();
    }

    vk::SharedShaderModule& GetShared()
    {
        return mShaderModule;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedShaderModule mShaderModule;
    vk::ShaderModuleCreateInfo mShaderInfo;
};

RENDER_TYPE_WRAPPER( Shader )