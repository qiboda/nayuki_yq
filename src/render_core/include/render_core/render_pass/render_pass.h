#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API RenderPass : public IRAII
{
  public:
    RenderPass();
    virtual ~RenderPass() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreateRenderPass( const vk::RenderPassCreateInfo& renderPassInfo )
    {
        NY_ASSERT( mDevice )

        vk::RenderPass renderPass = mDevice->createRenderPass( renderPassInfo );
        mRenderPass = vk::SharedRenderPass( renderPass, mDevice );
    }

    void DestroyRenderPass()
    {
        if ( mRenderPass )
        {
            NY_ASSERT( mDevice )

            mDevice->destroyRenderPass( mRenderPass.get() );
            mRenderPass.reset();
        }
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::RenderPass GetRaw()
    {
        return mRenderPass.get();
    }

    vk::SharedRenderPass& GetShared()
    {
        return mRenderPass;
    }

  protected:
    vk::SharedDevice mDevice;

    vk::SharedRenderPass mRenderPass;
    vk::RenderPassCreateInfo mRenderPassInfo;
};

RENDER_TYPE_WRAPPER( RenderPass )