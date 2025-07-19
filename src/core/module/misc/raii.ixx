module;

#include <module_export.h>

export module core.misc.iraii;

import core.misc.non_copyable;

export class CORE_API IRAII : public NonCopyable
{
  public:
    virtual ~IRAII() override = default;

    /// 正式初始化。
    /// 如果需要传递参数,请提前设置。
    virtual void Initialize() = 0;

    /// 清理资源。
    /// 释放资源,并将指针置为nullptr。
    virtual void CleanUp() = 0;
};
