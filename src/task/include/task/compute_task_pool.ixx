module ;

#include <core/core.h>

export module compute_task_pool;

import stdexec_mod;

namespace TaskPoolDetail
{
export inline u32 GetMaxThreadNum()
{
    return std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1u;
}
} // namespace TaskPoolDetail

export class TASK_API ComputeTaskPool
{
  public:
    static inline constexpr u32 DefaultThreadCount = 8;

  public:
    ComputeTaskPool()
        : mStaticThreadPool( DefaultThreadCount )
        , mScheduler( mStaticThreadPool.get_scheduler() )
    {
    }

  public:
    sender auto Schedule()
    {
        return schedule( mScheduler );
    }

    template <typename Func>
    sender auto Then( Func&& func )
    {
        return schedule( mScheduler ) | then( func );
    }

    template <typename... Func>
    sender auto WhenAll( Func&&... func )
    {
        return schedule( mScheduler ) | when_all( func... );
    }

  protected:
    static_thread_pool mStaticThreadPool;
    static_thread_pool::scheduler mScheduler;
};