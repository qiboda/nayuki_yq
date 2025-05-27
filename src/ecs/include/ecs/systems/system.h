#pragma once

#include "ecs/systems/system_param.h"
#include <concepts>
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <tuple>
#include <type_traits>

enum class FuncType
{
    Invalid,
    Func,
    FuncPtr,
    FuncRef,
};

struct FnTraitBase
{
    using ReturnType = void;
    using ArgsTypeList = std::tuple<>;
    constexpr static auto FuncType = FuncType::Invalid;
};

template <typename Fn>
concept IsFnTrait = std::is_base_of_v<FnTraitBase, Fn>;

template <typename Func>
struct FnTrait : FnTraitBase
{
};

template <typename Ret, typename... Args>
struct FnTrait<Ret( Args... )> : FnTraitBase
{
    using ReturnType = Ret;
    using ArgsTypeList = std::tuple<Args...>;
    constexpr static auto FuncType = FuncType::Func;
};

template <typename Ret, typename... Args>
struct FnTrait<Ret ( * )( Args... )> : FnTraitBase
{
    using ReturnType = Ret;
    using ArgsTypeList = std::tuple<Args...>;
    constexpr static auto FuncType = FuncType::FuncPtr;
};

template <typename Ret, typename... Args>
struct FnTrait<Ret ( & )( Args... )> : FnTraitBase
{
    using ReturnType = Ret;
    using ArgsTypeList = std::tuple<Args...>;
    constexpr static auto FuncType = FuncType::FuncRef;
};

template <typename Func>
using FnReturnType = typename FnTrait<Func>::ReturnType;

template <typename Func>
using FnArgsTypeList = typename FnTrait<Func>::ArgsTypeList;

template <typename... T>
consteval bool FnArgsTypeListIsSystemParamSet( std::tuple<T...> )
{
    return IsSystemParamSetConcept<T...>;
}

template <typename Func>
inline constexpr bool IsSystem =
    std::same_as<FnReturnType<Func>, void> && FnArgsTypeListIsSystemParamSet( FnArgsTypeList<Func>() );

template <typename Func>
concept IsSystemConcept = IsSystem<Func>;

class ECS_API System
{
  public:
    System();

  public:
    template <typename Func>
        requires IsSystem<Func>
    static System From( Func func )
    {
        using ArgsTypeList = FnArgsTypeList<Func>;
        constexpr size_t kParamsCount = ArgsTypeList::size();

        System system;
        system.set_system_func( std::move( func ), std::make_index_sequence<kParamsCount>{} );
        return system;
    }

  private:
    template <typename Func, size_t... Index>
        requires IsSystem<Func>
    void set_system_func( Func func, std::index_sequence<Index...> )
    {
        // using ArgsTypeList = FnArgsTypeList<Func>;
        // TODO:传入函数的真实参数值。
        mSystemFunc = [func = std::move( func )]() { func(); };
    }

    std::function<void()> mSystemFunc;
};
