#pragma once

#include <core/minimal.h>
#include <core/minimal.h>

template <typename TBase, typename TDerived>
inline constexpr bool IsStrictDerivedValue = std::is_base_of_v<TBase, TDerived> && !std::is_same_v<TBase, TDerived>;

template <typename TBase, typename TDerived>
struct IsStrictDerived
{
    static inline constexpr bool Value = IsStrictDerivedValue<TBase, TDerived>;
};

template <typename TBase, typename TDerived>
concept IsStrictDerivedConcept = IsStrictDerivedValue<TBase, TDerived>;
