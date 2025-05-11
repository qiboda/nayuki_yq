#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class FrameBuffer : public IRAII
{
  public:
    FrameBuffer();
    virtual ~FrameBuffer() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreateFrameBuffer( const vk::FramebufferCreateInfo &frameBufferInfo )
    {
        NY_ASSERT( mDevice )

        vk::Framebuffer frameBuffer = mDevice->createFramebuffer( frameBufferInfo );
        mFrameBuffer = vk::SharedFramebuffer( frameBuffer, mDevice );
    }

    void DestroyFrameBuffer()
    {
        mDevice->destroyFramebuffer( mFrameBuffer.get() );
        mFrameBuffer.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::Framebuffer GetRaw()
    {
        return mFrameBuffer.get();
    }

    vk::SharedFramebuffer GetShared()
    {
        return mFrameBuffer;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedFramebuffer mFrameBuffer;
};