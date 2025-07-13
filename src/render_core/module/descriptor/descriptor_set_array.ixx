module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>
#include <core/macro/macro.h>

export module render_core.descriptor_set_array;

import core;
import core.misc.iraii;
import render_core.descriptor_pool;
import render_core.descriptor_set;
import render_core.descriptor_set_layout;
import std;

export class RENDER_CORE_API DescriptorSetArray : public IRAII
{
  public:
    DescriptorSetArray()
    {
    }

    virtual ~DescriptorSetArray() override
    {
    }

  public:
    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

  public:
    void CreateDescriptorSetArray( DescriptorPool descriptorPool, DescriptorSetLayout descriptorSetLayout, u32 count );

    void DestroyDescriptorSetArray();

  public:
    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::DescriptorSet GetRaw( size_t index )
    {
        return mDescriptorSets[index].GetRaw();
    }

    vk::SharedDescriptorSet GetShared( size_t index )
    {
        return mDescriptorSets[index].GetShared();
    }

  protected:
    std::vector<vk::DescriptorSet> GetRaw()
    {
        std::vector<vk::DescriptorSet> rawDescriptorSets;
        rawDescriptorSets.reserve( mDescriptorSets.size() );
        for ( auto& descriptorSet : mDescriptorSets )
        {
            rawDescriptorSets.push_back( descriptorSet.GetRaw() );
        }
        return rawDescriptorSets;
    }

  protected:
    vk::SharedDevice mDevice;
    std::vector<DescriptorSet> mDescriptorSets;
    vk::SharedDescriptorPool mDescriptorPool;
};

void DescriptorSetArray::CreateDescriptorSetArray( DescriptorPool descriptorPool,
                                                   DescriptorSetLayout descriptorSetLayout,
                                                   u32 count )
{
    NY_ASSERT( mDevice )

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = descriptorPool.GetRaw();
    auto rawDescriptorSetLayout = descriptorSetLayout.GetRaw();
    allocInfo.pSetLayouts = &rawDescriptorSetLayout;
    allocInfo.descriptorSetCount = count;

    std::vector<vk::DescriptorSet> descriptorSets = mDevice->allocateDescriptorSets( allocInfo );
    mDescriptorSets.resize( count );
    for ( size_t i = 0; i < count; ++i )
    {
        mDescriptorSets[i].SetDescriptorSet(
            vk::SharedDescriptorSet( descriptorSets[i], mDevice, descriptorPool.GetShared() ) );
    }
}

void DescriptorSetArray::DestroyDescriptorSetArray()
{
    std::vector<vk::DescriptorSet> rawDescriptorSetArray = GetRaw();
    mDevice->freeDescriptorSets( mDescriptorPool.get(),
                                 static_cast<u32>( rawDescriptorSetArray.size() ),
                                 rawDescriptorSetArray.data() );
    mDescriptorSets.clear();
}

RENDER_TYPE_WRAPPER( DescriptorSetArray )