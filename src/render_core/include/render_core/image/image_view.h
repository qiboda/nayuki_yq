#pragma once

#include <core/core.h>
#include <render_core/render_core.h>

class RENDER_CORE_API ImageView : public IRAII
{
  public:
    ImageView();
    virtual ~ImageView() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreateImageView( const vk::ImageViewCreateInfo& imageViewInfo )
    {
        NY_ASSERT( mDevice )

        vk::ImageView imageView = mDevice->createImageView( imageViewInfo );
        mImageView = vk::SharedImageView( imageView, mDevice );
    }

    void DestroyImageView()
    {
        mDevice->destroyImageView( mImageView.get() );
        mImageView.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::ImageView GetRaw()
    {
        return mImageView.get();
    }

    vk::SharedImageView& GetShared()
    {
        return mImageView;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedImageView mImageView;
    vk::ImageViewCreateInfo mImageViewInfo;
};

RENDER_TYPE_WRAPPER( ImageView )