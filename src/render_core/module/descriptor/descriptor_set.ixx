module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>

export module render_core.descriptor_set;

import std;
import core;
import core.misc.iraii;

export class RENDER_CORE_API DescriptorSet : public IRAII
{
  public:
    DescriptorSet()
    {
    }

    DescriptorSet( vk::SharedDescriptorSet&& descriptorSet )
        : mDescriptorSet( std::move( descriptorSet ) )
    {
    }

    virtual ~DescriptorSet() override
    {
    }

    DescriptorSet( DescriptorSet&& descriptorSet )
    {
        mDevice = std::move( descriptorSet.mDevice );
        mDescriptorSet = std::move( descriptorSet.mDescriptorSet );
    }

  public:
    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

  public:
    void BindDescriptorSet()
    {
    }

    void UnbindDescriptorSet()
    {
    }

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    void SetDescriptorSet( vk::SharedDescriptorSet descriptorSet )
    {
        mDescriptorSet = descriptorSet;
    }

    vk::DescriptorSet GetRaw()
    {
        return mDescriptorSet.get();
    }

    vk::SharedDescriptorSet& GetShared()
    {
        return mDescriptorSet;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedDescriptorSet mDescriptorSet;
};

RENDER_TYPE_WRAPPER( DescriptorSet )