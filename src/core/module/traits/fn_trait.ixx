module;

#include <module_export.h>

export module core.trait;

import core;

export enum class FuncKind : u8 {
    Invalid,

    Func,
    FuncPtr,
    FuncRef,

    Method,
    MethodPtr,
};

export struct ECS_API FnInfo
{
    virtual ~FnInfo() = default;
};

/// 其中Param没有类型限定符，Args则保留了类型限定符。
export template <typename Ret, typename... Params>
struct FnTraitBase
{
    using ReturnType = Ret;

    using ParamsTypeTuple = std::tuple<Params...>;
    using DecayedParamsTypeTuple = std::tuple<std::decay_t<Params>...>;

    // 自定义参数类型包
    template <usize I>
    using Param = std::tuple_element_t<I, std::tuple<Params...>>;
    template <usize I>
    using DecayedParam = std::decay_t<Param<I>>;

    // 参数数量
    static constexpr usize ArgCount = sizeof...( Params );

    template <template <typename...> typename T>
    using ApplyParamsTo = T<Params...>;
    template <template <typename...> typename T>
    using ApplyDecayedParamsTo = T<std::decay_t<Params>...>;
};

export template <typename Func>
struct FnTrait : FnTraitBase<void>
{
    static_assert( false );
    using FuncType = Func;
    constexpr static auto FuncKind = FuncKind::Invalid;
};

export template <typename Ret, typename... Args>
struct FnTrait<Ret( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret( Args... );
    constexpr static auto FuncKind = FuncKind::Func;
};

export template <typename Ret, typename... Args>
struct FnTrait<Ret ( * )( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret ( * )( Args... );
    constexpr static auto FuncKind = FuncKind::FuncPtr;
};

export template <typename Ret, typename... Args>
struct FnTrait<Ret ( & )( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret ( & )( Args... );
    constexpr static auto FuncKind = FuncKind::FuncRef;
};

export template <typename TClass, typename Ret, typename... Args>
struct FnTrait<Ret ( TClass::* )( Args... )> : FnTraitBase<Ret, Args...>
{
    using FuncType = Ret ( TClass::* )( Args... );
    constexpr static auto FuncKind = FuncKind::MethodPtr;
};

export template <typename T>
concept IsFnTrait = std::same_as<FnTrait<typename T::FuncType>, T>;

export template <typename Func>
using FnReturnType = typename FnTrait<Func>::ReturnType;

export template <typename Func>
using FnParamsTypeTuple = typename FnTrait<Func>::ParamsTypeTuple;
export template <typename Func>
using FnDecayedParamsTypeTuple = typename FnTrait<Func>::DecayedParamsTypeTuple;

export template <typename Func, usize Index>
using FnParamType = typename FnTrait<Func>::template Param<Index>;
export template <typename Func, usize Index>
using FnDecayedParamType = typename FnTrait<Func>::template DecayedParam<Index>;

export template <typename Func>
static constexpr inline usize FnArgCount = FnTrait<Func>::ArgCount;

export template <typename Func, template <typename...> typename T>
using ApplyFnParamsTo = typename FnTrait<Func>::template ApplyParamsTo<T>;
export template <typename Func, template <typename...> typename T>
using ApplyFnDecayedParamsTo = typename FnTrait<Func>::template ApplyDecayedParamsTo<T>;
