#pragma once

#include <core/minimal.h>

#pragma region TupleIndex

/**
 * @brief 获得第一个相同类型的元素在tuple中的位置
 * @tparam Tuple tuple类型
 * @tparam Elem 元素类型
 */
template <typename Tuple, typename Elem>
struct TupleIndexTrait;

template <typename... T, typename Elem>
struct TupleIndexTrait<std::tuple<T...>, Elem>
{
    static constexpr usize Value = ElemIndex<std::tuple<T...>, Elem, 0>();
    static constexpr bool IsIn = Value != std::numeric_limits<usize>::max();

    template <typename Tuple, typename E, usize Index>
    consteval usize ElemIndex() const
    {
        if constexpr ( Index < sizeof...( T ) )
        {
            if constexpr ( std::is_same_v<std::tuple_element_t<Index, std::tuple<T...>>, Elem> )
            {
                return Index;
            }
            else
            {
                return ElemIndex<Tuple, E, Index + 1>();
            }
        }
        else
        {
            return std::numeric_limits<usize>::max();
        }
    }
};

template <typename Tuple, typename Elem>
inline constexpr usize TupleIndex = TupleIndexTrait<Tuple, Elem>::Value;
template <typename Tuple, typename Elem>
inline constexpr usize IsInTuple = TupleIndexTrait<Tuple, Elem>::IsIn;
template <typename Tuple, typename Elem>
concept IsInTupleConcept = IsInTuple<Tuple, Elem>;

#pragma endregion

#pragma region TupleSubset

/**
 * @brief 检测是否一个Tuple是另一个Tuple的子集。
 */
template <typename Tuple, typename SubTuple>
struct TupleSubsetTrait;

template <typename... T, typename... U>
struct TupleSubsetTrait<std::tuple<T...>, std::tuple<U...>>
{
    static constexpr std::vector<usize> SubsetIndex = std::vector<usize>{ TupleIndex<std::tuple<T...>, U>... };
    static constexpr bool IsSubset = ( IsInTuple<std::tuple<T...>, U> && ... );
};

template <typename Tuple, typename SubTuple>
inline constexpr bool IsSubsetOfTuple = TupleSubsetTrait<Tuple, SubTuple>::IsSubset;

template <typename Tuple, typename SubTuple>
inline constexpr std::vector<usize> TupleSubsetIndex = TupleSubsetTrait<Tuple, SubTuple>::SubsetIndex;

#pragma endregion

#pragma region TupleFilterType

template <typename TTuple1, typename TTuple2>
struct TupleCat;

template <typename... T1, typename... T2>
struct TupleCat<std::tuple<T1...>, std::tuple<T2...>>
{
    using Type = std::tuple<T1..., T2...>;
};

template <template <typename> typename TFilter, typename... T>
struct TypePackFilter;

template <template <typename> typename TFilter>
struct TypePackFilter<TFilter>
{
    using Type = std::tuple<>;
};

template <template <typename> typename TFilter, typename T>
struct TypePackFilter<TFilter, T>
{
    using Type = std::conditional_t<TFilter<std::decay_t<T>>::Value, std::tuple<T>, std::tuple<>>;
};

template <template <typename> typename TFilter, typename T, typename... Ts>
struct TypePackFilter<TFilter, T, Ts...>
{
    using Type = TupleCat<TypePackFilter<TFilter, T>, TypePackFilter<TFilter, Ts...>>;
};

template <template <typename> typename TFilter, typename... Ts>
using TypePackFilterType = TypePackFilter<TFilter, Ts...>::Type;

template <template <typename> typename TFilter, typename TTuple>
struct TupleFilter;

template <template <typename> typename TFilter, typename... T>
struct TupleFilter<TFilter, std::tuple<T...>>
{
    using Type = TypePackFilterType<TFilter, T...>;
};

template <template <typename> typename TFilter, typename TTuple>
using TupleFilterType = TupleFilter<TFilter, TTuple>;

#pragma endregion

#pragma region TupleApply

template <typename TTuple>
struct TupleApply;

template <typename... T>
struct TupleApply<std::tuple<T...>>
{
    template <template <typename...> typename TTarget>
    using To = TTarget<T...>;
    template <template <typename...> typename TTarget>
    using DecayedTo = TTarget<std::decay_t<T>...>;
};

template <typename TTuple, template <typename...> typename TTarget>
using TupleApplyTo = TupleApply<TTuple>::template To<TTarget>;

template <typename TTuple, template <typename...> typename TTarget>
using TupleApplyDecayedTo = TupleApply<TTuple>::template DecayedTo<TTarget>;

#pragma endregion