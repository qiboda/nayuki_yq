#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

/// 所有组件的基类
struct ECS_API Component
{
  public:
    Component();

    virtual ~Component() = default;
};
