export module core.concepts.tuple;

import core;
import std;

#pragma region TupleIndex

/**
 * @brief 获得第一个相同类型的元素在tuple中的位置
 * @tparam Tuple tuple类型
 * @tparam Elem 元素类型
 */
export template <typename Tuple, typename Elem>
struct TupleIndexTrait;

export template <typename... T, typename Elem>
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

export template <typename Tuple, typename Elem>
inline constexpr usize TupleIndex = TupleIndexTrait<Tuple, Elem>::Value;
export template <typename Tuple, typename Elem>
inline constexpr usize IsInTuple = TupleIndexTrait<Tuple, Elem>::IsIn;
export template <typename Tuple, typename Elem>
concept IsInTupleConcept = IsInTuple<Tuple, Elem>;

#pragma endregion

#pragma region TupleSubset

/**
 * @brief 检测是否一个Tuple是另一个Tuple的子集。
 */
export template <typename Tuple, typename SubTuple>
struct TupleSubsetTrait;

export template <typename... T, typename... U>
struct TupleSubsetTrait<std::tuple<T...>, std::tuple<U...>>
{
    static constexpr std::vector<usize> SubsetIndex = std::vector<usize>{ TupleIndex<std::tuple<T...>, U>... };
    static constexpr bool IsSubset = ( IsInTuple<std::tuple<T...>, U> && ... );
};

export template <typename Tuple, typename SubTuple>
inline constexpr bool IsSubsetOfTuple = TupleSubsetTrait<Tuple, SubTuple>::IsSubset;

export template <typename Tuple, typename SubTuple>
inline constexpr std::vector<usize> TupleSubsetIndex = TupleSubsetTrait<Tuple, SubTuple>::SubsetIndex;

#pragma endregion

#pragma region TupleFilterType

export template <typename TTuple1, typename TTuple2>
struct TupleCat;

export template <typename... T1, typename... T2>
struct TupleCat<std::tuple<T1...>, std::tuple<T2...>>
{
    using Type = std::tuple<T1..., T2...>;
};

export template <typename TTuple1, typename TTuple2>
using TupleCatType = TupleCat<TTuple1, TTuple2>::Type;

export template <template <typename> typename TFilter, typename... T>
struct TypePackFilter;

export template <template <typename> typename TFilter, typename... Ts>
using TypePackFilterType = TypePackFilter<TFilter, Ts...>::Type;

export template <template <typename> typename TFilter>
struct TypePackFilter<TFilter>
{
    using Type = std::tuple<>;
};

export template <template <typename> typename TFilter, typename T>
struct TypePackFilter<TFilter, T>
{
    using Type = std::conditional_t<TFilter<std::decay_t<T>>::Value, std::tuple<T>, std::tuple<>>;
};

export template <template <typename> typename TFilter, typename T, typename... Ts>
struct TypePackFilter<TFilter, T, Ts...>
{
    using Type = TupleCatType<TypePackFilterType<TFilter, T>, TypePackFilterType<TFilter, Ts...>>;
};

export template <template <typename> typename TFilter, typename TTuple>
struct TupleFilter;

export template <template <typename> typename TFilter, typename... T>
struct TupleFilter<TFilter, std::tuple<T...>>
{
    using Type = TypePackFilterType<TFilter, T...>;
};

export template <template <typename> typename TFilter, typename TTuple>
using TupleFilterType = TupleFilter<TFilter, TTuple>::Type;

#pragma endregion

#pragma region TupleApply

export template <typename TTuple>
struct TupleApply;

export template <typename... T>
struct TupleApply<std::tuple<T...>>
{
    template <template <typename...> typename TTarget>
    using To = TTarget<T...>;
    template <template <typename...> typename TTarget>
    using DecayedTo = TTarget<std::decay_t<T>...>;
};

export template <typename TTuple, template <typename...> typename TTarget>
using TupleApplyTo = TupleApply<TTuple>::template To<TTarget>;

export template <typename TTuple, template <typename...> typename TTarget>
using TupleApplyDecayedTo = TupleApply<TTuple>::template DecayedTo<TTarget>;

#pragma endregion