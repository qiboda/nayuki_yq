#pragma once

#include "core/macro/macro.h"
#include "ecs/systems/system_concept.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <IsSystemConcept T>
class SystemState;

class ISystemState : public NonCopyable
{
  public:
    virtual ~ISystemState() = default;

    template <IsSystemConcept Func>
    const SystemState<Func> *Downcast( Func func ) const
    {
        UNUSED_VAR( func );
        return static_cast<const SystemState<Func>*>( this );
    }
};

/**
 * @brief 缓存System的状态, 例如SystemParam的缓存，更具体一些，比如 Local<T>的参数
 */
template <IsSystemConcept Func>
class ECS_API SystemState : public ISystemState
{
    // FnArgs to State
    using SystemParamStateListType = ApplyFnParamsTo<Func, AllSystemParamStateTypes>;

  public:
    SystemState()
        : SystemParamStates{}
    {
    }

    template <usize Index>
    FnParamType<Func, Index>::State &GetParamState()
    {
        return std::get<Index>( SystemParamStates );
    }

    template <usize Index>
    const FnParamType<Func, Index>::State &GetParamState() const
    {
        return std::get<Index>( SystemParamStates );
    }

  protected:
    SystemParamStateListType SystemParamStates;
};