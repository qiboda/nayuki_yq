#pragma once

#include <core/core.h>

/// 所有组件的基类
struct ECS_API Component
{
  public:
    Component();

    virtual ~Component() = default;
};
