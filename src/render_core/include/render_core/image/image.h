#pragma once

#include "render_core/image/image_view.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API Image : public IRAII
{
  public:
    Image();
    virtual ~Image() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

  public:
    void CreateImage( const vk::ImageCreateInfo& imageInfo )
    {
        NY_ASSERT( mDevice )

        vk::Image image = mDevice->createImage( imageInfo );
        mImage = vk::SharedImage( image, mDevice );
    }

    void DestroyImage()
    {
        mDevice->destroyImage( mImage.get() );
        mImage.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::Image GetRaw()
    {
        return mImage.get();
    }

    vk::SharedImage& GetShared()
    {
        return mImage;
    }

    SharedImageView GetImageView()
    {
        return mImageView;
    }

    void SetImageView( SharedImageView imageView )
    {
        mImageView = imageView;
    }

    vk::SharedDevice GetDevice()
    {
        return mDevice;
    }

  protected:
    vk::SharedDevice mDevice;

    vk::SharedImage mImage;
    vk::ImageCreateInfo mImageInfo;

    SharedImageView mImageView;
};

RENDER_TYPE_WRAPPER( Image )