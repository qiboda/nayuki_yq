
#include "task/compute_task_pool.h"
#include <exec/static_thread_pool.hpp>

ComputeTaskPool::ComputeTaskPool()
    : mStaticThreadPool( DefaultThreadCount )
    , mScheduler( mStaticThreadPool.get_scheduler() )
{
}
