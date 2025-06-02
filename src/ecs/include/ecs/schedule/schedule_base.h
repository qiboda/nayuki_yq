/// 分为多个阶段，每个阶段一个调度图，
/// 每个调度图的节点，可以设定前后顺序和依赖关系。
#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

/// 多个schedule，支持组成一个图，但是不支持嵌套。
class ECS_API ScheduleBase
{
  public:
    ScheduleBase();
};