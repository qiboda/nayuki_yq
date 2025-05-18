#pragma once

#include "module_export.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <type_traits>

/// 所有组件的基类
struct ECS_API Component
{
  public:
    Component();
};

class ComponentTypeRegistry;

struct ECS_API ComponentId
{
    friend class ComponentTypeRegistry;

    u32 id = 0;

    // 会隐式生成 != 比较操作。
    bool operator==( const ComponentId &other ) const = default;

  private:
    u32 GenNext()
    {
        return id++;
    }
};

// 用于获取组件id
// 使用静态变量缓存，接口易用，性能更高。
class ECS_API ComponentTypeRegistry
{
  public:
    template <typename T>
    static ComponentId Get()
    {
        static const ComponentId sId = ComponentId{ .id = sCounter.GenNext() };
        return sId;
    }

  private:
    static inline ComponentId sCounter;
};

template <typename T>
concept IsComponentConcept = std::is_base_of_v<Component, T>;

template <IsComponentConcept T>
struct ComponentTrait
{
    static constexpr size_t Size = sizeof( T );
    static constexpr size_t Align = alignof( T );
};
