#pragma once

#include <exec/static_thread_pool.hpp>
#include <core/core.h>

class TASK_API AsyncComputeTaskPool
{
  public:
    AsyncComputeTaskPool();

  protected:
    exec::static_thread_pool mStaticThreadPool;
    exec::static_thread_pool::scheduler mScheduler;
};