module;

#include "module_export.h"
#include <exec/static_thread_pool.hpp>

export module io_task_pool;

import core;

export class TASK_API IOTaskPool
{
  public:
    IOTaskPool()
        : mStaticThreadPool( 8 )
        , mScheduler( mStaticThreadPool.get_scheduler() )
    {
    }

  protected:
    exec::static_thread_pool mStaticThreadPool;
    exec::static_thread_pool::scheduler mScheduler;
};