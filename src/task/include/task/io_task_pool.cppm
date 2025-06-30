
#include <core/core.h>

export module io_task_pool;
import stdexec_mod;

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