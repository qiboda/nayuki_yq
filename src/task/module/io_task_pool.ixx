module;

#include "module_export.h"

export module io_task_pool;

import stdexec;
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
    static_thread_pool mStaticThreadPool;
    static_thread_pool::scheduler mScheduler;
};