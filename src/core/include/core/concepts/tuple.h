#pragma once

#include <core/minimal.h>

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
static inline constexpr usize TupleIndex = TupleIndexTrait<Tuple, Elem>::Value;
template <typename Tuple, typename Elem>
static inline constexpr usize IsInTuple = TupleIndexTrait<Tuple, Elem>::IsIn;
template <typename Tuple, typename Elem>
concept IsInTupleConcept = IsInTuple<Tuple, Elem>;

/**
 * @brief 获得第一个相同类型的元素在tuple中的位置
 * @tparam Tuple tuple类型
 * @tparam Elem 元素类型
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
static inline constexpr bool IsSubsetOfTuple = TupleSubsetTrait<Tuple, SubTuple>::IsSubset;

template <typename Tuple, typename SubTuple>
static inline constexpr std::vector<usize> TupleSubsetIndex = TupleSubsetTrait<Tuple, SubTuple>::SubsetIndex;