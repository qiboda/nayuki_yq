module;

#include "module_export.h"

export module async_compute_task_pool;

import core;
import stdexec;

export class TASK_API AsyncComputeTaskPool
{
  public:
    AsyncComputeTaskPool()
        : mStaticThreadPool( 8 )
        , mScheduler( mStaticThreadPool.get_scheduler() )
    {
    }

  protected:
    static_thread_pool mStaticThreadPool;
    static_thread_pool::scheduler mScheduler;
};