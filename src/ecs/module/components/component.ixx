module;

#include "module_export.h"

export module ecs.components.component;

/// 所有组件的基类
export struct ECS_API Component
{
  public:
    Component();

    virtual ~Component() = default;
};
