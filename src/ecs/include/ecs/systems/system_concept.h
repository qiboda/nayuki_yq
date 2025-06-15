#pragma once

#include <core/minimal.h>
#include "core/traits/fn_trait.h"
#include "ecs/components/component_info.h"
#include "ecs/systems/system_param.h"

template <typename... T>
struct AllSystemParamsWrapper
{
    static inline constexpr bool value = AllSystemParamRefsValue<T...>;
};

template <typename Func>
inline constexpr bool IsSystem = std::same_as<FnReturnType<Func>, void> && FnArgCount<Func> > 0 &&
                                 ApplyFnParamsTo<Func, AllSystemParamsWrapper>::value;

template <typename Func>
concept IsSystemConcept = IsSystem<Func>;

template <IsSystemConcept Func>
struct SystemTrait
{
    using ReturnType = FnReturnType<Func>;

    using Params = FnParamsTypeTuple<Func>;
    using DecayedParams = FnDecayedParamsTypeTuple<Func>;

    constexpr inline static bool ParamCount = FnArgCount<Func>;

    template <usize Index>
    using ParamType = FnParamType<Func, Index>;
    template <usize Index>
    using DecayedParamType = FnDecayedParamType<Func, Index>;

    template <usize... I>
    static consteval std::vector<std::pair<ComponentId, bool>> CombineReadWrite( std::index_sequence<I...> )
    {
        std::vector<std::pair<ComponentId, bool>> readWrite;
        ( readWrite.insert( readWrite.end(), CombineReadWriteImpl<I>() ), ... );
        return readWrite;
    }

    template <usize TIndex>
    static consteval std::vector<std::pair<ComponentId, bool>> CombineReadWriteImpl()
    {
        if constexpr ( HasComponentsReadWrite<DecayedParamType<TIndex>> )
        {
            return SystemParamTrait<DecayedParamType<TIndex>>::ComponentsReadWrite;
        }
        else
        {
            return {};
        }
    }

    static constexpr inline std::vector<std::pair<ComponentId, bool>> ReadWriteComponents =
        CombineReadWrite( std::make_index_sequence<ParamCount>{} );
};