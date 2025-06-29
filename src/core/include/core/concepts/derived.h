#pragma once

#include <type_traits>

#pragma region IsStrictDerived

template <typename TBase, typename TDerived>
inline constexpr bool IsStrictDerivedValue = std::is_base_of_v<TBase, TDerived> && !std::is_same_v<TBase, TDerived>;

template <typename TBase, typename TDerived>
struct IsStrictDerived
{
    static inline constexpr bool Value = IsStrictDerivedValue<TBase, TDerived>;
};

template <typename TBase, typename TDerived>
concept IsStrictDerivedConcept = IsStrictDerivedValue<TBase, TDerived>;

#pragma endregion // IsStrictDerived

#pragma region IsSameValue

template <typename TA, typename TB>
inline constexpr bool IsSameValue = std::is_same_v<TA, TB>;

template <typename TA, typename TB>
struct IsSame
{
    static inline constexpr bool Value = IsSameValue<TA, TB>;
};

template <typename TA, typename TB>
concept IsSameConcept = IsSameValue<TA, TB>;

#pragma endregion // IsSameValue

#pragma region IsNot

template <typename T>
inline constexpr bool IsNotValue = !T::Value;

template <typename T>
struct IsNot
{
    static inline constexpr bool Value = IsNotValue<T>;
};

template <typename T>
concept IsNotConcept = IsNotValue<T>;

#pragma endregion // IsSameValue