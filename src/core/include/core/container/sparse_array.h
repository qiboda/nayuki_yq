#pragma once

#include <concepts>
#include <core/minimal.h>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

/// TKey 决定了 TValue的最大数量。
template <std::unsigned_integral TKey, typename TValue>
class SparseArray
{
  public:
    SparseArray() = default;

    SparseArray( size_t size )
        : mDenseArray( size )
        , mSparseKeyArray( size )
    {
    }

  public:
    void Add( TKey key, TValue value )
    {
        if ( key >= mSparseKeyArray.size() )
        {
            mSparseKeyArray.resize( key + 1 );
        }

        if ( mSparseKeyArray[key] == std::numeric_limits<TKey>::max() )
        {
            mSparseKeyArray[key] = static_cast<TKey>( mDenseArray.size() );
            mDenseArray.push_back( value );
        }
    }

    std::optional<TValue> Remove( TKey key )
    {
        if ( key < mSparseKeyArray.size() )
        {
            auto index = mSparseKeyArray[key];
            if ( index < mDenseArray.size() )
            {
                std::swap( mDenseArray[index], mDenseArray.back() );
                mSparseKeyArray[key] = std::numeric_limits<TKey>::max();
                TValue value = std::move( mDenseArray.back() );
                mDenseArray.pop_back();
                return value;
            }
        }
        return std::nullopt;
    }

    inline const TValue *TryGet( TKey key ) const
    {
        if ( key < mSparseKeyArray.size() )
        {
            auto index = mSparseKeyArray[key];
            if ( index < mDenseArray.size() )
            {
                return mDenseArray[index];
            }
        }
        return nullptr;
    }

    inline TValue *TryGet( TKey key )
    {
        if ( key < mSparseKeyArray.size() )
        {
            auto index = mSparseKeyArray[key];
            if ( index < mDenseArray.size() )
            {
                return mDenseArray[index];
            }
        }
        return nullptr;
    }

    inline TValue &Get( TKey key )
    {
        return mDenseArray[mSparseKeyArray[key]];
    }

    inline const TValue &Get( TKey key ) const
    {
        return mDenseArray[mSparseKeyArray[key]];
    }

  public:
    inline usize Size() const
    {
        return mDenseArray.size();
    }

    inline usize Capacity() const
    {
        return mSparseKeyArray.size();
    }

    inline void Clear()
    {
        mDenseArray.clear();
        mSparseKeyArray.clear();
    }

  private:
    std::vector<TValue> mDenseArray;
    std::vector<TKey> mSparseKeyArray;
};