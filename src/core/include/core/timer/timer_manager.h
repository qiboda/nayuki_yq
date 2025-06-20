#pragma once

#include <core/container/heap.h>
#include <core/delegate/delegate_interface.h>
#include <core/misc/singleton.h>
#include <core/timer/timer_handle.h>
#include <core/minimal.h>
#include <unordered_set>

DECL_SINGLE_DElEGATE_TYPE( TimerDelegate );

enum class TimerState : u8
{
    None,
    Active,
    Paused
};

struct TimerData
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

static const TimerData InvalidTimerData;

/**
 * TODO: add a check if the current frame has pass.
 */
class CORE_API TimerManager : public Singleton<TimerManager>
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
