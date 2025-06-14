
#include "task/async_compute_task_pool.h"

AsyncComputeTaskPool::AsyncComputeTaskPool()
    : mStaticThreadPool( 8 )
    , mScheduler( mStaticThreadPool.get_scheduler() )
{
}
