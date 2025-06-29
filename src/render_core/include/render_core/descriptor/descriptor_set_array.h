#pragma once

#include "render_core/descriptor/descriptor_pool.h"
#include "render_core/descriptor/descriptor_set.h"
#include "render_core/descriptor/descriptor_set_layout.h"

#include <vector>

class RENDER_CORE_API DescriptorSetArray : public IRAII
{
  public:
    DescriptorSetArray();
    virtual ~DescriptorSetArray() override;

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

RENDER_TYPE_WRAPPER( DescriptorSetArray )