#include "core/type_def.h"
#include <coroutine>
#include <iostream>

// 一个简单的 Awaitable 类型
// 对 co_await 的控制。
struct SimpleAwaitable
{
    bool await_ready() const noexcept
    {
        std::cout << "await_ready\n";
        return false; // false 表示需要挂起
    }

    void await_suspend( std::coroutine_handle<> h ) const noexcept
    {
        std::cout << "await_suspend\n";
        h.resume(); // 立刻恢复，模拟异步任务完成
    }

    int await_resume() const noexcept
    {
        std::cout << "await_resume\n";
        return 42; // 协程恢复时的返回值
    }
};

// 协程的返回类型，需要定义 promise_type
struct MyTask
{
    // 协程 函数的标志。 控制了整个协程函数的使用。
    struct promise_type
    {
        // 回去返回类型的对象
        MyTask get_return_object()
        {
            return {};
        }

        std::suspend_never initial_suspend() noexcept
        {
            return {};
        } // 立即开始

        std::suspend_never final_suspend() noexcept
        {
            return {};
        } // 不挂起

        // co_return
        void return_void()
        {
        }

        // co_yield, 返回 Awaitable 类型的对象。
        void yield_value()
        {
        }

        // co_return value; 结果存入 promise_type 内部。返回类型必须是 void。
        // void return_value(T value)
        // {
        // }

        void unhandled_exception()
        {
        }
    };

    bool await_ready() const noexcept
    {
        std::cout << "await_ready\n";
        return false; // false 表示需要挂起
    }

    void await_suspend( std::coroutine_handle<> h ) const noexcept
    {
        std::cout << "await_suspend\n";
        h.resume(); // 立刻恢复，模拟异步任务完成
    }

    int await_resume() const noexcept
    {
        std::cout << "await_resume\n";
        return 42; // 协程恢复时的返回值
    }
};

MyTask ano_coroutine()
{
    // 每当写 co_await 或者 co_return 时, 编译器会自动生成一个 promise_type 实例。
    // 如此，没有写return，却有返回值才不会报错。
    co_await SimpleAwaitable{};
    std::cout << "ano coroutine: " << std::endl;
}

// 使用协程函数
// 通过返回类型决定这个函数是协程函数。
MyTask my_coroutine()
{
    i32 result = co_await ano_coroutine();
    std::cout << "co_await returned: " << result << "\n";
}

int main()
{
    my_coroutine(); // 执行协程
}
