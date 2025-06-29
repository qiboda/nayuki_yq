#pragma once

#include "core/macro/macro.h"
#include "ecs/registry.h"
#include "ecs/systems/system_param.h"

template <typename T>
class LocalState : public SystemParamState
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
    Local( State* state )
        : mState{ state }
    {
    }

    static Local<T> From( Registry* registry, State* state )
    {
        UNUSED_VARS( registry );
        return Local<T>( state );
    }

    T& operator*()
    {
        return mState->value;
    }

    const T& operator*() const
    {
        return mState->value;
    }

  protected:
    State* mState = nullptr;
};

template <typename T>
struct SystemParamTrait<Local<T>>
{
};
