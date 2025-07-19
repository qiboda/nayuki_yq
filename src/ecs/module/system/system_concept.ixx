module;

#include "module_export.h"

export module ecs:system_concepts;

import std;
import core.type;
import core.trait;
import :system_param;
import :component_info;

export template <typename... T>
struct AllSystemParamsWrapper
{
    static inline constexpr bool value = AllSystemParamRefsValue<T...>;
};

export template <typename Func>
inline constexpr bool IsSystem = std::same_as<FnReturnType<Func>, void> && FnArgCount<Func> > 0 &&
                                 ApplyFnParamsTo<Func, AllSystemParamsWrapper>::value;

export template <typename Func>
concept IsSystemConcept = IsSystem<Func>;

export template <IsSystemConcept Func>
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