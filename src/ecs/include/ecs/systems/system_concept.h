#pragma once

#include <concepts>
#include <core/minimal.h>
#include <type_traits>
#include "ecs/systems/system_param.h"

enum class FuncKind
{
    Invalid,
    Func,
    FuncPtr,
    FuncRef,
};

/// 其中Param没有类型限定符，Args则保留了类型限定符。
template <typename Ret, typename... Args>
struct FnTraitBase
{
    using ReturnType = Ret;

    using ArgsTypeTuple = std::tuple<Args...>;
    using ParamsTypeTuple = std::tuple<std::decay_t<Args>...>;

    // 自定义参数类型包
    template <usize I>
    using Arg = std::tuple_element_t<I, std::tuple<Args...>>;
    template <usize I>
    using Param = std::decay_t<Arg<I>>;

    // 参数数量
    static constexpr usize ArgCount = sizeof...( Args );

    template <template <typename...> typename T>
    using ApplyArgsTo = T<Args...>;
    template <template <typename...> typename T>
    using ApplyParamsTo = T<std::decay_t<Args>...>;
};

template <typename Func>
struct FnTrait : FnTraitBase<void>
{
    // static_assert( false );
    using FuncType = Func;
    constexpr static auto FuncKind = FuncKind::Invalid;
};

template <typename Ret, typename... Args>
struct FnTrait<Ret( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret( Args... );
    constexpr static auto FuncKind = FuncKind::Func;
};

template <typename Ret, typename... Args>
struct FnTrait<Ret ( * )( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret ( * )( Args... );
    constexpr static auto FuncKind = FuncKind::FuncPtr;
};

template <typename Ret, typename... Args>
struct FnTrait<Ret ( & )( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret ( & )( Args... );
    constexpr static auto FuncKind = FuncKind::FuncRef;
};

template <typename T>
concept IsFnTrait = std::same_as<FnTrait<typename T::FuncType>, T>;

template <typename Func>
using FnReturnType = typename FnTrait<Func>::ReturnType;

template <typename Func>
using FnArgsTypeTuple = typename FnTrait<Func>::ArgsTypeTuple;
template <typename Func>
using FnParamsTypeTuple = typename FnTrait<Func>::ParamsTypeTuple;

template <typename Func, usize Index>
using FnArgType = typename FnTrait<Func>::template Arg<Index>;
template <typename Func, usize Index>
using FnParamType = typename FnTrait<Func>::template Param<Index>;

template <typename Func, template <typename...> typename T>
using ApplyFnArgsTo = typename FnTrait<Func>::template ApplyArgsTo<T>;
template <typename Func, template <typename...> typename T>
using ApplyFnParamsTo = typename FnTrait<Func>::template ApplyParamsTo<T>;

template <typename... T>
struct AllSystemParamsWrapper
{
    static inline constexpr bool value = ( AllSystemParams<std::decay_t<T>...> ) && ( std::is_reference_v<T> && ... );
};

template <typename Func>
inline constexpr bool IsSystem =
    std::same_as<FnReturnType<Func>, void> && ApplyFnArgsTo<Func, AllSystemParamsWrapper>::value;

template <typename Func>
concept IsSystemConcept = IsSystem<Func>;
