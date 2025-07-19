module;

#include "module_export.h"
#include "core/macro/macro.h"

export module ecs:system_param_local;

import :system_param;
import :registry;

export template <typename T>
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
export template <typename T>
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

export template <typename T>
struct SystemParamTrait<Local<T>>
{
};
