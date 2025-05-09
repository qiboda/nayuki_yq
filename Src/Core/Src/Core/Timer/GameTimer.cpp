// ***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
// ***************************************************************************************

#include "Core/Timer/GameTimer.h"

GameTimer::GameTimer( GameTimerState GameTimerState /* = GameTimerState::Running */ )
    : mPrevTimePoint( mClock.now() ), mCurrTimePoint( mPrevTimePoint ), mDeltaDuration( 0 ), mRunningDuration( 0 ),
      mRunningTimePoint( mCurrTimePoint ), mPausedDuration( 0 ), mPausedTimePoint( mCurrTimePoint ),
      mTimerState( GameTimerState )
{
}

f32 GameTimer::TotalTime() const { return ( mPausedDuration + mRunningDuration ).count(); }

f32 GameTimer::RunningTotalTime() const { return mRunningDuration.count(); }

f32 GameTimer::PausedTotalTime() const { return mPausedDuration.count(); }

f32 GameTimer::DeltaTime() const { return mDeltaDuration.count(); }

void GameTimer::Start()
{
    mStop = false;
    if ( mStart == false && mTimerState == GameTimerState::Paused )
    {
        mStart = true;
    }
}

void GameTimer::Stop()
{
    mStart = false;
    if ( mStop == false && mTimerState == GameTimerState::Running )
    {
        mStop = true;
    }
}

void GameTimer::Tick()
{
    mPrevTimePoint = mCurrTimePoint;
    mCurrTimePoint = mClock.now();
    mDeltaDuration = mCurrTimePoint - mPrevTimePoint;

    switch ( mTimerState )
    {
    case GameTimerState::Running:
    {
        if ( mStop )
        {
            mTimerState = GameTimerState::Paused;

            mPausedTimePoint = mCurrTimePoint;

            mStop = false;
        }
        mRunningDuration += mDeltaDuration;
        break;
    }
    case GameTimerState::Paused:
    {
        if ( mStart )
        {
            mTimerState = GameTimerState::Running;

            mRunningTimePoint = mCurrTimePoint;

            mStart = false;
        }
        mPausedDuration += mDeltaDuration;
        break;
    }
    }
}
