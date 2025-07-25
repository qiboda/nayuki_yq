/// 分为多个阶段，每个阶段一个调度图，
/// 每个调度图的节点，可以设定前后顺序和依赖关系。
module;

#include "module_export.h"

export module ecs:schedule_base;

import core;
import std;
import core.misc.non_copyable;

/// 多个schedule，支持组成一个图，但是不支持嵌套。
export class ECS_API ScheduleBase : public std::enable_shared_from_this<ScheduleBase>, public NonCopyable
{
  public:
    ScheduleBase();
    virtual ~ScheduleBase() = default;

  public:
    void Init( class Registry* registry );

  public:
    /// 添加一个系统节点配置到调度图中。
    void AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config );

    /// 添加一个系统集节点配置到调度图中。
    void AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config );

    std::shared_ptr<class ScheduleGraph> GetScheduleGraph()
    {
        return mScheduleGraph;
    }

    const std::shared_ptr<class ScheduleGraph> GetScheduleGraph() const
    {
        return mScheduleGraph;
    }

    void BeforeRun( class Registry* registry );

    virtual void Run( class Registry* registry );

  protected:
    std::shared_ptr<class ScheduleGraph> mScheduleGraph = nullptr;
};
