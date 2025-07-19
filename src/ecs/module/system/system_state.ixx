module;

#include "module_export.h"
#include "core/macro/macro.h"

export module ecs:system_state;

import std;
import core.misc.non_copyable;
import core.trait;
import core.type;
import :system_concepts;
import :system_param;

export template <IsSystemConcept T>
class SystemState;

export class ECS_API ISystemState : public NonCopyable
{
  public:
    virtual ~ISystemState() = default;

    template <IsSystemConcept Func>
    const SystemState<Func>* Downcast( Func func ) const
    {
        UNUSED_VARS( func );
        return static_cast<const SystemState<Func>*>( this );
    }
};

/**
 * @brief 缓存System的状态, 例如SystemParam的缓存，更具体一些，比如 Local<T>的参数
 */
export template <IsSystemConcept Func>
class SystemState : public ISystemState
{
    // FnArgs to State
    using SystemParamStateListType = ApplyFnDecayedParamsTo<Func, AllSystemParamStateTypes>;

  public:
    SystemState()
        : SystemParamStates{}
    {
    }

    template <usize Index>
    FnDecayedParamType<Func, Index>::State& GetParamState()
    {
        return std::get<Index>( SystemParamStates );
    }

    template <usize Index>
    const FnDecayedParamType<Func, Index>::State& GetParamState() const
    {
        return std::get<Index>( SystemParamStates );
    }

  protected:
    SystemParamStateListType SystemParamStates;
};