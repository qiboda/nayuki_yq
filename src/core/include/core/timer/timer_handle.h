#pragma once

#include <core/macro/macro.h>
#include <core/type_def.h>

namespace TimerHandle_Detail
{
static u64 gTimerHandleIndex = 0u;
}

class TimerHandle
{
  public:
    friend class TimerManager;

  public:
    using NoneIndex = NoneIndex<u64>;

    static constexpr NoneIndex NONE_INDEX = NoneIndex();

  public:
    struct Hash
    {
        constexpr u64 operator()( const TimerHandle& timerHandle ) const
        {
            return timerHandle.mDataIndex;
        }
    };

  public:
    constexpr TimerHandle()
        : mDataIndex( NONE_INDEX )
    {
    }

  public:
    bool IsValid() const
    {
        return mDataIndex != NONE_INDEX;
    }

    void Invalid()
    {
        mDataIndex = NONE_INDEX;
    }

  public:
    friend bool operator==( const TimerHandle& LHS, const TimerHandle& RHS )
    {
        return LHS.mDataIndex == RHS.mDataIndex;
    }

    friend bool operator!=( const TimerHandle& LHS, const TimerHandle& RHS )
    {
        return ( LHS == RHS ) == false;
    }

  private:
    void NextIndex()
    {
        if ( TimerHandle_Detail::gTimerHandleIndex == std::numeric_limits<u64>::max() )
        {
            TimerHandle_Detail::gTimerHandleIndex = 0u;
        }
        ++TimerHandle_Detail::gTimerHandleIndex;
        SetDataIndex( TimerHandle_Detail::gTimerHandleIndex );
    }

    // Only access through the friend class.
    // Avoid modification by external classes.
    void SetDataIndex( u64 dataIndex )
    {
        NY_PRE_CONDITION( 0u < dataIndex && dataIndex < std::numeric_limits<decltype( mDataIndex )>::max(), "" )
        mDataIndex = dataIndex;
    }

  private:
    u64 mDataIndex;
};
