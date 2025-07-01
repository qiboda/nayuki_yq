module;

export module core.fsm:state;

import std;
import core;
import core.misc;

export template <typename TStateId, typename TFSMContext>
class State : public NonCopyable
{

  public:
    virtual ~State() override
    {
    }

  public:
    static TStateId GetStateId_S()
    {
        return TStateId{};
    }

    virtual TStateId GetStateId() const
    {
        return GetStateId_S();
    }

#pragma region update
  public:
    void Update( std::shared_ptr<TFSMContext>& fsmContext, f32 DeltaSeconds )
    {
        OnUpdate( fsmContext, DeltaSeconds );
    }

  public:
    virtual void OnUpdate( std::shared_ptr<TFSMContext>& fsmContext, f32 DeltaSeconds ) = 0;
#pragma endregion update

#pragma region enter
  private:
    void Enter( std::shared_ptr<TFSMContext>& fsmContext )
    {
        OnEnterPre( fsmContext );
        OnEnter( fsmContext );
        OnEnterPost( fsmContext );
    }

  public:
    virtual void OnEnterPre( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
    virtual void OnEnter( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
    virtual void OnEnterPost( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
#pragma endregion enter

#pragma region leave
  private:
    void Leave( std::shared_ptr<TFSMContext>& fsmContext )
    {
        OnLeavePre( fsmContext );
        OnLeave( fsmContext );
        OnLeavePost( fsmContext );
    }

  public:
    virtual void OnLeavePre( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
    virtual void OnLeave( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
    virtual void OnLeavePost( std::shared_ptr<TFSMContext>& fsmContext ) = 0;
#pragma endregion leave
};
