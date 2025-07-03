module;

#include "core/logger/log.h"
// #include "core/logger/logger.h"
import core.logger;
#include "render_core/render_core.h"

export module render_core.pipeline;

import render_core.pipeline_layout;


export class RENDER_CORE_API Pipeline : public IRAII
{
  public:
    Pipeline()
    {
    }

    virtual ~Pipeline() override
    {
    }

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreatePipeline( const vk::GraphicsPipelineCreateInfo& pipelineInfo )
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

    vk::SharedPipeline& GetShared()
    {
        return mPipeline;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedPipeline mPipeline;
    vk::GraphicsPipelineCreateInfo mPipelineInfo;

    PipelineLayout mPipelineLayout;
};

RENDER_TYPE_WRAPPER( Pipeline )