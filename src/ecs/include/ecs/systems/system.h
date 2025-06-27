#pragma once

#include "core/macro/macro.h"
#include "core/registry/id.h"
#include "ecs/components/component_info.h"
#include "ecs/systems/system_concept.h"
#include "ecs/systems/system_state.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API SystemId : public Id
{
    friend class SystemIdRegistry;

  public:
    static const SystemId Invalid;

  public:
    friend bool operator==( const SystemId& lhs, const SystemId& rhs )
    {
        return lhs.mId == rhs.mId;
    }

    friend bool operator<( const SystemId& lhs, const SystemId& rhs )
    {
        return lhs.mId < rhs.mId;
    }
};

template <>
struct std::hash<SystemId>
{
    size_t operator()( SystemId systemId ) const noexcept
    {
        return std::hash<usize>()( systemId.Index() );
    }
};

class ECS_API SystemIdRegistry
{
  public:
    template <IsSystemConcept Func>
    static SystemId Get( Func func )
    {
        auto it = mFuncToIdMap.find( reinterpret_cast<void*>( func ) );
        if ( it != mFuncToIdMap.end() )
        {
            return it->second;
        }

        return SystemId::Invalid;
    }

    template <IsSystemConcept Func>
    static SystemId Add( Func func )
    {
        auto systemId = Next();
        mFuncToIdMap.emplace( reinterpret_cast<void*>( func ), systemId );
        return systemId;
    }

    template <IsSystemConcept Func>
    static SystemId GetOrAdd( Func func )
    {
        auto systemId = Get( func );
        if ( systemId != SystemId::Invalid )
        {
            return systemId;
        }

        return Add( func );
    }

  protected:
    static SystemId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static SystemId sId;

    static std::unordered_map<void*, SystemId> mFuncToIdMap;
};

template <IsSystemConcept Func>
class System;

/**
 * @brief System的接口类
 */
class ECS_API ISystem : public NonCopyable
{
  public:
    virtual ~ISystem() = default;

  public:
    virtual void Run( class Registry* registry ) = 0;
    virtual const ISystemState& GetSystemStateBase() const = 0;

    template <IsSystemConcept Func>
    const System<Func>* Downcast( Func func ) const
    {
        UNUSED_VARS( func );
        return static_cast<const System<Func>*>( this );
    }

    virtual bool write_conflict( const std::unique_ptr<ISystem>& other ) const = 0;
};

static inline const std::unique_ptr<ISystem> sNullISystem = nullptr;

/**
 * @brief 每个Func的System的特化类。
 */
template <IsSystemConcept Func>
class System : public ISystem
{
  public:
    constexpr System( Func func )
    {
        using ArgsTypeTuple = FnParamsTypeTuple<Func>;
        constexpr size_t kArgsCount = std::tuple_size_v<ArgsTypeTuple>;
        static_assert( kArgsCount > 0 );
        SetSystemFunc( func, std::make_index_sequence<kArgsCount>{} );
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

    virtual bool write_conflict( const std::unique_ptr<ISystem>& other ) const override
    {
        UNUSED_VARS( other );
        return false; // 默认不冲突
    }

  private:
    template <size_t... Index>
    constexpr void SetSystemFunc( Func func, std::index_sequence<Index...> )
    {
        // TODO:传入函数的真实参数值。 SystemState
        mSystemFunc = [func, this]( class Registry* registry )
        {
            using ParamsTypeTuple = FnDecayedParamsTypeTuple<Func>;

            ParamsTypeTuple tuple = std::make_tuple(
                ( FnDecayedParamType<Func, Index>::From( registry, &mSystemState.template GetParamState<Index>() ), ... ) );
            // 将第Index个SystemParam构造出值
            std::apply( func, tuple );
        };
    }

    std::function<void( class Registry* )> mSystemFunc;

    /**
     * @brief
     * true 表示写，false 表示只读
     */
    std::unordered_map<ComponentId, bool> mComponentReadWriteMap;

    SystemState<Func> mSystemState;
};
