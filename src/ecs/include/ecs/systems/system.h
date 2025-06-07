#pragma once

#include "core/macro/macro.h"
#include "ecs/systems/system_concept.h"
#include "ecs/systems/system_state.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <functional>
#include <limits>
#include <tuple>

struct ECS_API SystemId
{
    friend std::hash<SystemId>;

  public:
    friend bool operator==( const SystemId& lhs, const SystemId& rhs )
    {
        return lhs.mId == rhs.mId;
    }

    friend bool operator<( const SystemId& lhs, const SystemId& rhs )
    {
        return lhs.mId < rhs.mId;
    }

  public:
    SystemId()
        : mId( std::numeric_limits<usize>::max() )
    {
    }

    void Set()
    {
        mId = sIdGenerator++;
    }

  private:
    usize mId;

    static inline usize sIdGenerator = 0;
};

static const SystemId InvalidSystemId = SystemId();

template <>
struct std::hash<SystemId>
{
    size_t operator()( SystemId systemId ) const noexcept
    {
        return std::hash<usize>()( systemId.mId );
    }
};

template <IsSystemConcept Func>
class System;

/**
 * @brief System的接口类
 */
class ISystem : public NonCopyable
{
  public:
    virtual ~ISystem() = default;

  public:
    virtual void Run( class Registry* registry ) = 0;
    virtual const ISystemState& GetSystemStateBase() const = 0;

    template <IsSystemConcept Func>
    const System<Func>* Downcast( Func func ) const
    {
        UNUSED_VAR( func );
        return static_cast<const System<Func>*>( this );
    }
};

static const std::unique_ptr<ISystem> sNullISystem = nullptr;

/**
 * @brief 每个Func的System的特化类。
 */
template <IsSystemConcept Func>
class System : public ISystem
{
  public:
    System( Func func )
    {
        using ArgsTypeTuple = FnArgsTypeTuple<Func>;
        constexpr size_t kArgsCount = std::tuple_size_v<ArgsTypeTuple>;
        SetSystemFunc( std::move( func ), std::make_index_sequence<kArgsCount>{} );
    }

    virtual ~System() = default;

    virtual void Run( class Registry* registry ) override
    {
        mSystemFunc( registry );
    }

    virtual const ISystemState& GetSystemStateBase() const override
    {
        return mSystemState;
    }

    const SystemState<Func>& GetSystemState() const
    {
        return mSystemState;
    }

  private:
    template <size_t... Index>
    void SetSystemFunc( Func func, std::index_sequence<Index...> )
    {
        // TODO:传入函数的真实参数值。 SystemState
        mSystemFunc = [func = std::move( func ), this]( class Registry* registry )
        {
            using ParamsTypeTuple = FnParamsTypeTuple<Func>;

            ParamsTypeTuple tuple = std::make_tuple(
                ( FnParamType<Func, Index>::From( registry, mSystemState.template GetParamState<Index>() ), ... ) );
            // 将第Index个SystemParam构造出值
            std::apply( func, tuple );
        };
    }

    std::function<void( class Registry* )> mSystemFunc;

    SystemState<Func> mSystemState;
};
