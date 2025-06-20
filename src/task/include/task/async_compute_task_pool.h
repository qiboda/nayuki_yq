#pragma once

#include <core/minimal.h>
#include <exec/static_thread_pool.hpp>
#include <task/minimal.h>

class TASK_API AsyncComputeTaskPool
{
  public:
    AsyncComputeTaskPool();

  protected:
    exec::static_thread_pool mStaticThreadPool;
    exec::static_thread_pool::scheduler mScheduler;
};