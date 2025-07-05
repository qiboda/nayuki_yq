module;

#include <module_export.h>
#include <core/macro/macro.h>
#include <fmt/format.h>

export module core.timer.manager;

import std;
import core;
import core.misc.singleton;
import core.delegate;
import core.container.heap;

export using TimerDelegate = SingleDelegateDecl<>;

export enum class TimerState : u8 { None, Active, Paused };

namespace TimerHandle_Detail
{
static u64 gTimerHandleIndex = 0u;
}

export class TimerHandle
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

export struct TimerData
{
    constexpr TimerData()
    {
    }

    bool bloop = false;
    f32 intervalTime = 0.0f;
    // When this is less than zero, the timer delegate is called.
    f32 expireTime = 0.0f;
    // TimerHandle timerHandle;
    TimerDelegate timerDelegate;
    TimerState timerState = TimerState::None;

    bool IsValid() const
    {
        return timerState != TimerState::None;
    }
};

export inline const TimerData InvalidTimerData;

/**
 * TODO: add a check if the current frame has pass.
 */
export class CORE_API TimerManager : public Singleton<TimerManager>
{
  private:
    // make constructor protected access permission.
    friend class Singleton<TimerManager>;

  private:
    struct TimerHandleExpireTimeGreater
    {
        bool operator()( const TimerHandle& LHS, const TimerHandle& RHS ) const
        {
            const TimerData& timerDataLHS = TimerManager::GetInstance().GetTimerData( LHS );
            const TimerData& timerDataRHS = TimerManager::GetInstance().GetTimerData( RHS );
            return timerDataLHS.expireTime > timerDataRHS.expireTime;
        }
    };

  private:
    // The TimerData with the smallest ExpireTime is top.
    using TimerHandleMinHeap = Heap<TimerHandle, TimerHandleExpireTimeGreater>;
    // TODO: sparse array or hash map
    using TimerDataHashMap = std::unordered_map<TimerHandle, TimerData, TimerHandle::Hash>;
    using TimerHashSet = std::unordered_set<TimerHandle, TimerHandle::Hash>;

  protected:
    TimerManager()
        : Singleton()
        , mInternalTime( 0.0f )
    {
    }

  public:
    void Tick( f32 deltaSeconds );

  public:
    TimerHandle SetTimer( f32 intervalTime, TimerDelegate timerDelegate, bool bLoop = false, bool bActive = true );
    void InvalidTimer( const TimerHandle& timerHandle );

    void PauseTimer( const TimerHandle& timerHandle );
    void ActiveTimer( const TimerHandle& timerHandle );

  private:
    const TimerData& GetTimerData( const TimerHandle& timerHandle ) const;
    TimerData& GetTimerData( const TimerHandle& timerHandle );

  private:
    TimerDataHashMap mTimerDataHashMap;
    TimerHandleMinHeap mActiveTimerMinHeap;
    TimerHashSet mPausedTimerHashSet;

    f32 mInternalTime;
};
