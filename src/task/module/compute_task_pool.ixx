module;

#include "module_export.h"
#include <exec/static_thread_pool.hpp>

export module compute_task_pool;

import std;
import core;

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
    stdexec::sender auto Schedule()
    {
        return stdexec::schedule( mScheduler );
    }

    template <typename Func>
    stdexec::sender auto Then( Func&& func )
    {
        return stdexec::schedule( mScheduler ) | stdexec::then( func );
    }

    template <typename... Func>
    stdexec::sender auto WhenAll( Func&&... func )
    {
        return stdexec::schedule( mScheduler ) | stdexec::when_all( func... );
    }

  protected:
    exec::static_thread_pool mStaticThreadPool;
    exec::static_thread_pool::scheduler mScheduler;
};