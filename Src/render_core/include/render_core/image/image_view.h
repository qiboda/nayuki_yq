#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class ImageView : public IRAII
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

    void CreateImageView( const vk::ImageViewCreateInfo &imageViewInfo )
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

    vk::SharedImageView GetShared()
    {
        return mImageView;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedImageView mImageView;
    vk::ImageViewCreateInfo mImageViewInfo;
};

using SharedImageView = std::shared_ptr<ImageView>;
using UniqueImageView = std::unique_ptr<ImageView>;