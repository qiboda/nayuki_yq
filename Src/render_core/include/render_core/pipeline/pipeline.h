#pragma once

#include "core/logger/logger.h"
#include "render_core/pipeline/pipeline_layout.h"
#include "render_core/render_core.h"
#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API Pipeline : public IRAII
{
  public:
    Pipeline();
    virtual ~Pipeline() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreatePipeline( const vk::GraphicsPipelineCreateInfo &pipelineInfo )
    {
        NY_ASSERT( mDevice )

        vk::ResultValue<vk::Pipeline> pipelineValue = mDevice->createGraphicsPipeline( nullptr, pipelineInfo );
        if ( pipelineValue.result != vk::Result::eSuccess )
        {
            NY_LOG_WARN( LogRenderCore,
                         "Failed to create graphics pipeline: {}",
                         vk::to_string( pipelineValue.result ) );
            return;
        }
        mPipeline = vk::SharedPipeline( pipelineValue.value, mDevice );
    }

    void DestroyPipeline()
    {
        mDevice->destroyPipeline( mPipeline.get() );
        mPipeline.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::Pipeline GetRaw()
    {
        return mPipeline.get();
    }

    vk::SharedPipeline GetShared()
    {
        return mPipeline;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedPipeline mPipeline;
    vk::GraphicsPipelineCreateInfo mPipelineInfo;

    PipelineLayout mPipelineLayout;
};