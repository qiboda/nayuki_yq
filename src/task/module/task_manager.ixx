module;

#include <module_export.h>

export module task_manager;

/**
 * @brief 分为三个task pool
 * 1. io thread pool（io密集型）
 * 2. compute thread pool(用于执行同步任务，计算密集型)
 * 3. async compute thread pool
 *    (用于执行异步任务，计算密集型，用于和io密集型任务混合使用，或者希望能够控制task的执行时使用)
 * 4. 还可以有其他的，比如后台任务。
 *
 * 之后还要考虑，检测是否是主线程，属于哪个线程池等。
 */
export class TASK_API TaskManager
{
  public:
    TaskManager()
    {
    }
};