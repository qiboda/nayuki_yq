#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <core/type_def.h>
#include <core/macro/macro.h>

/**
 * Heap: Implemented using std heap algorithm.
 *
 * TCompPred: std::less is the largest heap and the std::greater is the smallest
 * heap.
 *
 * Heap layout:
 *                  0
 *          1               7
 *      3       4       8       9
 *
 * Inner data layout:
 *      0 7 2 3 4 8 9
 */
#include <type_traits>

template <typename TElem, typename TCompPred = std::less<TElem>, typename TContainer = std::vector<TElem>> class Heap
{
    static_assert( std::is_floating_point_v<TElem> == false );

  public:
#pragma region TypeAlias
    using Iterator = typename TContainer::iterator;
    using ConstIterator = typename TContainer::const_iterator;
    using NoneIndex = NoneIndex<u64>;
#pragma endregion TypeAlias

    static constexpr NoneIndex NONE_INDEX = NoneIndex();

    struct HeapDetail
    {
        static constexpr u64 LeftElemIndex( u64 index, u64 size )
        {
            if ( size > index * 2 + 1 )
            {
                return index * 2 + 1;
            }
            return Heap::NONE_INDEX;
        }

        static constexpr u64 RightElemIndex( u64 index, u64 size )
        {
            if ( size > index * 2 + 2 )
            {
                return index * 2 + 2;
            }
            return Heap::NONE_INDEX;
        }
    };

  public:
#pragma region Constructor

    Heap() : mContainer()
    {
    }

    Heap( Iterator begin, Iterator end ) : mContainer( begin, end )
    {
        std::make_heap( mContainer.begin(), mContainer.end(), TCompPred() );
    }

#pragma endregion Constructor

  public:
#pragma region Operate

    void Push( const TElem &elem )
    {
        mContainer.push_back( elem );
        std::push_heap( mContainer.begin(), mContainer.end(), TCompPred() );
    }

    TElem Pop()
    {
        std::pop_heap( mContainer.begin(), mContainer.end(), TCompPred() );
        auto pop_value = mContainer.back();
        mContainer.pop_back();
        return pop_value;
    }

    const TElem &Top() const
    {
        return mContainer.front();
    }
    NON_CONST_MEM_FUN( Top )

    // TODO: change to remove_heap_elem() global function.
    bool Remove( const TElem &elem )
    {
        u64 index = GetElemIndex( elem );
        if ( index == NONE_INDEX )
        {
            return false;
        }

        mContainer[index] = std::move( mContainer.back() );
        mContainer.pop_back();

        DownHeapElem( index );
        return true;
    }

#pragma endregion Operate

    u64 GetElemIndex( const TElem &elem ) const
    {
        return GetElemIndexInternal( elem, 0u );
    }

    u64 Size() const
    {
        return mContainer.size();
    }

  private:
    u64 GetElemIndexInternal( const TElem &elem, const u64 index ) const
    {
        if ( index >= mContainer.size() )
        {
            return NONE_INDEX;
        }

        if ( elem == mContainer.at( index ) )
        {
            return index;
        }
        if ( TCompPred()( mContainer.at( index ), elem ) )
        {
            return NONE_INDEX;
        }

        u64 leftElemIndex = HeapDetail::LeftElemIndex( index, mContainer.size() );
        if ( leftElemIndex == NONE_INDEX )
        {
            return NONE_INDEX;
        }
        u64 leftIndex = GetElemIndexInternal( elem, leftElemIndex );
        if ( leftIndex != NONE_INDEX )
        {
            return leftIndex;
        }

        u64 rightElemIndex = HeapDetail::LeftElemIndex( index, mContainer.size() );
        if ( rightElemIndex == NONE_INDEX )
        {
            return NONE_INDEX;
        }
        u64 rightIndex = GetElemIndexInternal( elem, rightElemIndex );
        if ( rightIndex != NONE_INDEX )
        {
            return rightIndex;
        }

        return NONE_INDEX;
    }

    void DownHeapElem( u64 index )
    {
        if ( index >= mContainer.size() )
        {
            return;
        }

        u64 leftElemIndex = HeapDetail::LeftElemIndex( index, mContainer.size() );
        if ( leftElemIndex == NONE_INDEX )
        {
            return;
        }

        u64 rightElemIndex = HeapDetail::RightElemIndex( index, mContainer.size() );
        if ( rightElemIndex == NONE_INDEX )
        {
            rightElemIndex = leftElemIndex;
        }

        u64 UpElemIndex = NONE_INDEX;
        if ( TCompPred()( mContainer[leftElemIndex], mContainer[rightElemIndex] ) )
        {
            UpElemIndex = rightElemIndex;
        }
        else
        {
            UpElemIndex = leftElemIndex;
        }

        if ( TCompPred()( mContainer[index], mContainer[UpElemIndex] ) )
        {
            std::swap( mContainer[index], mContainer[UpElemIndex] );
            DownHeapElem( leftElemIndex );
        }
    }

  private:
    TContainer mContainer;
};
