module;

#include "module_export.h"
#include <render_core/render_core.h>
#include <render_core/macro.h>
#include <core/macro/macro.h>

export module render_core.pipeline_layout;

import std;
import core;
import core.misc.iraii;

export class RENDER_CORE_API PipelineLayout : public IRAII
{
  public:
    PipelineLayout();
    virtual ~PipelineLayout() override;

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

  protected:
    vk::SharedDevice mDevice;

    vk::SharedPipelineLayout mPipelineLayout;
};

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
RENDER_TYPE_WRAPPER( PipelineLayout )