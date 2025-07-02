export module core.fsm:transition;

import core.misc;
import std;
import core;

import :context;

// TODO: add transiiton interval and transition priority.
export template <typename TTransitionId, typename TStateId, typename TFSMContext>
class Transition : public NonCopyable
{
  public:
    Transition()
        : mEnterStateId{}
        , mLeaveStateId{}
    {
    }

    Transition( TStateId enterStateId, TStateId leaveStateId )
        : mEnterStateId( enterStateId )
        , mLeaveStateId( leaveStateId )
    {
    }

    virtual ~Transition() override
    {
    }

  public:
    virtual TTransitionId GetTransitionId() const = 0;

  public:
    TStateId GetEnterStateId() const
    {
        return mEnterStateId;
    }

    TStateId GetLeaveStateId() const
    {
        return mLeaveStateId;
    }

  public:
    virtual void StartTransition( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
    virtual void EndTransition( std::shared_ptr<TFSMContext>& fsmContext ) = 0;

  public:
    virtual bool CanTransition( std::shared_ptr<TFSMContext>& fsmContext ) = 0;

  protected:
    TStateId mEnterStateId;
    TStateId mLeaveStateId;

  protected:
    f32 mTransitionInterval = 0.0f;
    i8 mTransitionPriority = 0;
};
