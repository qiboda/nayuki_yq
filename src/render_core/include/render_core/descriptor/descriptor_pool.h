#pragma once

#include "core/macro/macro.h"
#include "module_export.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API DescriptorPool : public IRAII
{
  public:
    DescriptorPool();
    virtual ~DescriptorPool() override;

  public:
    void Initialize() override;
    void CleanUp() override;

  public:
    vk::DescriptorPool GetRaw()
    {
        return mDescriptorPool.get();
    }

    vk::SharedDescriptorPool& GetShared()
    {
        return mDescriptorPool;
    }

  public:
    void CreateDescriptorPool( const vk::DescriptorPoolCreateInfo& poolInfo )
    {
        NY_ASSERT( mDevice )

        vk::DescriptorPool descriptorPool = mDevice->createDescriptorPool( poolInfo );
        mDescriptorPool = vk::SharedDescriptorPool( descriptorPool, mDevice );
    }

    void DestroyDescriptorPool()
    {
        mDevice->destroyDescriptorPool( mDescriptorPool.get() );
        mDescriptorPool.reset();
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedDescriptorPool mDescriptorPool;
    vk::DescriptorPoolCreateInfo mPoolInfo;
};

RENDER_TYPE_WRAPPER( DescriptorPool )