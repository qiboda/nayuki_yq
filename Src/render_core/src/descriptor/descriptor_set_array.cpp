
#include "render_core/descriptor/descriptor_set_array.h"

DescriptorSetArray::DescriptorSetArray()
{
}

DescriptorSetArray::~DescriptorSetArray()
{
}

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
