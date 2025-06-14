
#include "task/io_task_pool.h"

IOTaskPool::IOTaskPool()
    : mStaticThreadPool( 8 )
    , mScheduler( mStaticThreadPool.get_scheduler() )
{
}
