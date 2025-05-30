#pragma once

#include "core/macro/macro.h"
#include "ecs/registry.h"
#include "ecs/systems/system_param.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename T>
class LocalState : SystemParamState
{
  public:
    LocalState()
        : value{}
    {
    }

    T value;
};

/**
 * @brief
 *   保存状态在System作用域。可以在多次system调用之间，传递数据。
 * @tparam T 任意类型
 */
template <typename T>
class Local : public SystemParam
{
  public:
    using State = LocalState<T>;

  public:
    Local( State &state )
        : mValue{ state.value }
    {
    }

    static Local<T> From( Registry *registry, State &state )
    {
        UNUSED_VAR( registry );
        return Local<T>( state );
    }

    T &mValue;
};

template <typename T>
struct SystemParamTrait<Local<T>>
{
};
