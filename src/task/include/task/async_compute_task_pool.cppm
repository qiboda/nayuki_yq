#include <core/core.h>

export module async_compute_task_pool;

export class TASK_API AsyncComputeTaskPool
{
  public:
    AsyncComputeTaskPool()
        : mStaticThreadPool( 8 )
        , mScheduler( mStaticThreadPool.get_scheduler() )
    {
    }

  protected:
    exec::static_thread_pool mStaticThreadPool;
    exec::static_thread_pool::scheduler mScheduler;
};