module;

#include "module_export.h"
#include "core/macro/macro.h"

export module ecs:system;

import :component_info;
import :system_concepts;
import :system_state;
import :registry;

import std;
import core.type;
import core.misc.non_copyable;
import core.trait;

export template <IsSystemConcept Func>
class System;

/**
 * @brief System的接口类
 */
export class ECS_API ISystem : public NonCopyable
{
  public:
    virtual ~ISystem() = default;

  public:
    virtual void Run( Registry* registry ) = 0;
    virtual const ISystemState& GetSystemStateBase() const = 0;

    template <IsSystemConcept Func>
    const System<Func>* Downcast( Func func ) const
    {
        UNUSED_VARS( func );
        return static_cast<const System<Func>*>( this );
    }

    virtual bool write_conflict( const std::unique_ptr<ISystem>& other ) const = 0;
};

export inline const std::unique_ptr<ISystem> sNullISystem = nullptr;

/**
 * @brief 每个Func的System的特化类。
 */
export template <IsSystemConcept Func>
class System : public ISystem
{
  public:
    constexpr System( Func func )
    {
        using ArgsTypeTuple = FnParamsTypeTuple<Func>;
        constexpr usize kArgsCount = std::tuple_size_v<ArgsTypeTuple>;
        static_assert( kArgsCount > 0 );
        SetSystemFunc( func, std::make_index_sequence<kArgsCount>{} );
    }

    virtual ~System() = default;

    virtual void Run( Registry* registry ) override
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

    virtual bool write_conflict( const std::unique_ptr<ISystem>& other ) const override
    {
        UNUSED_VARS( other );
        return false; // 默认不冲突
    }

  private:
    template <usize... Index>
    constexpr void SetSystemFunc( Func func, std::index_sequence<Index...> )
    {
        // TODO:传入函数的真实参数值。 SystemState
        mSystemFunc = [func, this]( Registry* registry )
        {
            using ParamsTypeTuple = FnDecayedParamsTypeTuple<Func>;

            ParamsTypeTuple tuple = std::make_tuple(
                ( FnDecayedParamType<Func, Index>::From( registry, &mSystemState.template GetParamState<Index>() ),
                  ... ) );
            // 将第Index个SystemParam构造出值
            std::apply( func, tuple );
        };
    }

    std::function<void( Registry* )> mSystemFunc;

    /**
     * @brief
     * true 表示写，false 表示只读
     */
    std::unordered_map<ComponentId, bool> mComponentReadWriteMap;

    SystemState<Func> mSystemState;
};
