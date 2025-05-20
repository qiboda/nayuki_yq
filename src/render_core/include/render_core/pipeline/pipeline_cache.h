#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API PipelineCache : public IRAII
{
  public:
    PipelineCache();
    virtual ~PipelineCache() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

    void CreatePipelineCache( const vk::PipelineCacheCreateInfo &pipelineCacheInfo )
    {
        NY_ASSERT( mDevice )

        vk::PipelineCache pipelineCache = mDevice->createPipelineCache( pipelineCacheInfo );
        mPipelineCache = vk::SharedPipelineCache( pipelineCache, mDevice );
    }

    void DestroyPipelineCache()
    {
        mDevice->destroyPipelineCache( mPipelineCache.get() );
        mPipelineCache.reset();
    }

    void SetDevice( vk::SharedDevice device )
    {
        mDevice = device;
    }

    vk::PipelineCache GetRaw()
    {
        return mPipelineCache.get();
    }

    vk::SharedPipelineCache &GetShared()
    {
        return mPipelineCache;
    }

  protected:
    vk::SharedDevice mDevice;
    vk::SharedPipelineCache mPipelineCache;
};

RENDER_TYPE_WRAPPER( PipelineCache )