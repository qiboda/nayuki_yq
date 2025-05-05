#include "Core/Timer/TimerManager.h"

void TimerManager::Tick(f32 deltaSeconds) {
    mInternalTime += deltaSeconds;

    std::vector<TimerHandle> pendingExecuteTimerHandles;

    while (mActiveTimerMinHeap.Size() > 0) {
        const TimerHandle &topTimerHandle = mActiveTimerMinHeap.Top();
        if (mTimerDataHashMap.at(topTimerHandle).expireTime > mInternalTime) {
            break;
        } else {
            const TimerHandle &popTimerHandle = mActiveTimerMinHeap.Pop();
            pendingExecuteTimerHandles.push_back(popTimerHandle);
        }
    }

    for (auto executeTimerHandle : pendingExecuteTimerHandles) {
        if (executeTimerHandle.IsValid()) {
            TimerData &timerData = mTimerDataHashMap.at(executeTimerHandle);

            // execute delegate
            i32 executeCount = 1;
            if (timerData.bloop) {
                f32 elapsedTime = mInternalTime - timerData.expireTime;
                executeCount +=
                    static_cast<i32>(elapsedTime / timerData.intervalTime);
            }
            for (i32 i = 0; i < executeCount; ++i) {
                timerData.timerDelegate.Execute();
            }

            // handle data
            if (timerData.bloop) {
                // LastInternalTime -------------- expired time ----
                // mInternalTime 0 1                  99.5
                timerData.expireTime = timerData.expireTime +
                                       executeCount * timerData.intervalTime;

                mActiveTimerMinHeap.Push(executeTimerHandle);
            } else {
                mTimerDataHashMap.erase(executeTimerHandle);
            }
        }
    }
}

TimerHandle TimerManager::SetTimer(f32 intervalTime,
                                   TimerDelegate timerDelegate, bool bLoop,
                                   bool bActive) {
    TimerData timerData;
    timerData.bloop = bLoop;
    timerData.expireTime = mInternalTime + intervalTime;
    timerData.intervalTime = intervalTime;
    timerData.timerDelegate = timerDelegate;
    // timerData.timerHandle = timerHandle;
    timerData.timerState = bActive ? TimerState::Active : TimerState::Paused;

    TimerHandle timerHandle;
    timerHandle.NextIndex();
    mTimerDataHashMap.emplace(timerHandle, std::move(timerData));

    if (bActive) {
        mActiveTimerMinHeap.Push(timerHandle);
    } else {
        mPausedTimerHashSet.insert(timerHandle);
    }
    return timerHandle;
}

void TimerManager::InvalidTimer(const TimerHandle &timerHandle) {
    if (timerHandle.IsValid()) {
        // TODO: to next frame to remove before executing delegate.
        mActiveTimerMinHeap.Remove(timerHandle);
        mPausedTimerHashSet.erase(timerHandle);
        mTimerDataHashMap.erase(timerHandle);
    }
}

void TimerManager::PauseTimer(const TimerHandle &timerHandle) {
    if (timerHandle.IsValid()) {
        if (mActiveTimerMinHeap.Remove(timerHandle)) {
            mPausedTimerHashSet.insert(timerHandle);

            mTimerDataHashMap.at(timerHandle).timerState = TimerState::Paused;
            mTimerDataHashMap.at(timerHandle).expireTime -= mInternalTime;
        }
    }
}

void TimerManager::ActiveTimer(const TimerHandle &timerHandle) {
    if (timerHandle.IsValid()) {
        auto PausedEraseCount = mPausedTimerHashSet.erase(timerHandle);
        if (PausedEraseCount > 0) {
            mActiveTimerMinHeap.Push(timerHandle);

            mTimerDataHashMap.at(timerHandle).timerState = TimerState::Active;
            mTimerDataHashMap.at(timerHandle).expireTime += mInternalTime;
        }
    }
}

inline const TimerData &
TimerManager::GetTimerData(const TimerHandle &timerHandle) const {
    if (timerHandle.IsValid()) {
        auto It = mTimerDataHashMap.find(timerHandle);
        if (It != mTimerDataHashMap.end()) {
            return It->second;
        }
    }
    return InvalidTimerData;
}

inline TimerData &TimerManager::GetTimerData(const TimerHandle &timerHandle) {
    return const_cast<TimerData &>(
        std::as_const(*this).GetTimerData(timerHandle));
}