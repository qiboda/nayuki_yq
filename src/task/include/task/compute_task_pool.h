#pragma once

#include <core/minimal.h>
#include <exec/static_thread_pool.hpp>
#include <task/minimal.h>

namespace TaskPoolDetail
{
inline u32 GetMaxThreadNum()
{
    return std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1u;
}
} // namespace TaskPoolDetail

class TASK_API ComputeTaskPool
{
  public:
    static inline constexpr u32 DefaultThreadCount = 8;

  public:
    ComputeTaskPool();

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