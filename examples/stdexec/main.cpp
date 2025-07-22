#include <iostream>
#include <stdexec/execution.hpp>
#include "exec/static_thread_pool.hpp"
#include <exec/task.hpp>
import core.type;

using namespace stdexec;

using stdexec::sync_wait;

exec::task<i32> async_compute_int( auto sender )
{
    co_await sender;
    co_return 1;
}

auto main() -> int
{
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    exec::numa_policy numa{ exec::no_numa_policy{} };

    // 固定线程数的线程池。
    exec::static_thread_pool ctx{ 8 };

    scheduler auto sch = ctx.get_scheduler(); // 1

    sender auto begin = schedule( sch ); // 2
    sender auto hi_again = then(         // 3
        begin,                           // 3
        []
        {                                               // 3
            std::cout << "Hello world! Have an int.\n"; // 3
            return 13;                                  // 3
        } );                                            // 3

    auto [i] = sync_wait( std::move( hi_again ) ).value(); // 5
    std::cout << "Result: " << i << std::endl;

    sender auto ano = schedule( sch ) | then( []
                                              {                                               // 3
                                                  std::cout << "Hello world! Have an int.\n"; // 3
                                                  return 13;                                  // 3
                                              } );                                            // 3

    auto [k] = sync_wait( std::move( ano ) ).value(); // 5
    std::cout << "Result: " << k << std::endl;
}