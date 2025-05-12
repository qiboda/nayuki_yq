#pragma once

#include <chrono>

#include <module_export.h>
#include <core/type_def.h>

enum class GameTimerState : u8
{
    Running,
    Paused
};

class CORE_API GameTimer
{
  public:
    explicit GameTimer( GameTimerState GameTimerState = GameTimerState::Running );

    f32 TotalTime() const;        // in seconds
    f32 RunningTotalTime() const; // in seconds
    f32 PausedTotalTime() const;  // in seconds

    f32 DeltaTime() const; // in seconds

    void Start(); // Call when paused.
    void Stop();  // Call when unpaused.
    void Tick();  // Call every frame.

  private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<f32>;

#pragma warning( disable : 4251 )
    // variable order is important.
    TimePoint mPrevTimePoint;
    TimePoint mCurrTimePoint;
    Duration mDeltaDuration;

    Duration mRunningDuration;
    TimePoint mRunningTimePoint;

    Duration mPausedDuration;
    TimePoint mPausedTimePoint;
#pragma warning( default : 4251 )
    // variable order is important.

    Clock mClock;

    bool mStart = false;
    bool mStop = false;

    GameTimerState mTimerState;
};
