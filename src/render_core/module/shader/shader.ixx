module;

#include <render_core/render_core.h>
#include "module_export.h"
#include <core/macro/macro.h>
#include <render_core/macro.h>

export module render_core.shader;

import core;
import core.misc.iraii;
import std;

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