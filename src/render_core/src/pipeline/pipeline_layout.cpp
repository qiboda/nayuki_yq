
#include "render_core/pipeline/pipeline_layout.h"
#include "core/macro/macro.h"

PipelineLayout::PipelineLayout()
{
}

PipelineLayout::~PipelineLayout()
{
}

void PipelineLayout::Initialize()
{
    NY_PRE_CONDITION( mDevice, "" )

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setSetLayoutCount( 0 );
    pipelineLayoutInfo.setPushConstantRangeCount( 0 );

    auto pipelineLayout = mDevice->createPipelineLayout( pipelineLayoutInfo );
    mPipelineLayout = vk::SharedPipelineLayout( pipelineLayout, mDevice );
}

void PipelineLayout::CleanUp()
{
    if ( mPipelineLayout )
    {
        NY_ASSERT( mDevice )

        mDevice->destroyPipelineLayout( mPipelineLayout.get() );
        mPipelineLayout.reset();
    }
}
