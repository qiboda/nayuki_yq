#pragma once

#include <core/core.h>
#include <render_core/render_core.h>

class RENDER_CORE_API DescriptorSetLayout : public IRAII
{
  public:
    DescriptorSetLayout();
    virtual ~DescriptorSetLayout() override;

  public:
    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

  public:
    void CreateDescriptorLayout( const vk::DescriptorSetLayoutCreateInfo& layoutInfo )
    {
        NY_ASSERT( mDevice )

        vk::DescriptorSetLayout descriptorSetLayout = mDevice->createDescriptorSetLayout( layoutInfo );
        mDescriptorSetLayout = vk::SharedDescriptorSetLayout( descriptorSetLayout, mDevice );
    }

    void DestroyDescriptorLayout()
    {
        mDevice->destroyDescriptorSetLayout( mDescriptorSetLayout.get() );
        mDescriptorSetLayout.reset();
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::DescriptorSetLayout GetRaw()
    {
        return mDescriptorSetLayout.get();
    }

    vk::SharedDescriptorSetLayout& GetShared()
    {
        return mDescriptorSetLayout;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedDescriptorSetLayout mDescriptorSetLayout;
    vk::DescriptorSetLayoutCreateInfo mLayoutInfo;
};

RENDER_TYPE_WRAPPER( DescriptorSetLayout )