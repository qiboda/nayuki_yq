#include <gtest/gtest.h>

#include <core/core.h>

#include <core/fsm/fsm.h>
#include <core/fsm/fsm_context.h>
#include <core/fsm/fsm_instance.h>

#include <core/fsm/state.h>

#include <core/fsm/transition.h>

enum class SkillStateId : u8
{
    Spell,
    Cast,
    Max
};

class SkillFSMContext : public FSMContext
{
  public:
    SkillFSMContext() : FSMContext() {}
    virtual ~SkillFSMContext() {}

  public:
    SkillStateId mCurStateId = SkillStateId::Spell;
};

class SkillState : public State<SkillStateId, SkillFSMContext>
{
  public:
    SkillState() {}
    virtual ~SkillState() override {}

  private:
};

class SkillState_Spell : public SkillState
{
  public:
    SkillState_Spell() : SkillState() {}
    virtual ~SkillState_Spell() override {}

  public:
    using SkillState::GetStateId;
    static SkillStateId GetStateId_S() { return SkillStateId::Spell; }

  public:
    virtual void OnUpdate( std::shared_ptr<SkillFSMContext> &fsmContext, f32 DeltaSeconds ) override
    {
        UNUSED_VAR( fsmContext );
        UNUSED_VAR( DeltaSeconds );
    }

    virtual void OnEnterPre( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnter( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnterPost( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePre( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeave( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePost( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }

  private:
};

class SkillState_Cast : public SkillState
{
  public:
    SkillState_Cast() : SkillState() {}
    virtual ~SkillState_Cast() override {}

  public:
    using SkillState::GetStateId;
    static SkillStateId GetStateId_S() { return SkillStateId::Cast; }

  public:
    virtual void OnUpdate( std::shared_ptr<SkillFSMContext> &fsmContext, f32 DeltaSeconds ) override
    {
        UNUSED_VAR( fsmContext );
        UNUSED_VAR( DeltaSeconds );
    }

    virtual void OnEnterPre( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnter( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnterPost( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePre( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeave( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePost( std::shared_ptr<SkillFSMContext> &fsmContext ) override { UNUSED_VAR( fsmContext ); }

  private:
};

enum class SkillTransitionId : u8
{
    Spell2Cast,
    Cast2Spell,
    Max,
};

class SkillTransition : public Transition<SkillTransitionId, SkillStateId, SkillFSMContext>
{
  public:
    using Super = Transition<SkillTransitionId, SkillStateId, SkillFSMContext>;

  public:
    SkillTransition() : Super() {}
    SkillTransition( SkillStateId enterStateId, SkillStateId leaveStateId ) : Super( enterStateId, leaveStateId ) {}
    virtual ~SkillTransition() override {}
};

class SkillTransition_Spell2Cast : public SkillTransition
{
  public:
    SkillTransition_Spell2Cast() : SkillTransition() {}
    SkillTransition_Spell2Cast( SkillStateId enterStateId, SkillStateId leaveStateId )
        : SkillTransition( enterStateId, leaveStateId )
    {
    }
    virtual ~SkillTransition_Spell2Cast() override {}

  public:
    virtual SkillTransitionId GetTransitionId() const override { return SkillTransitionId::Spell2Cast; }

  public:
    virtual void StartTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        EndTransition( fsmContext );
    }

    virtual void EndTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        if ( fsmContext )
        {
            fsmContext->mCurStateId = mLeaveStateId;
        }
    }

  public:
    // TODO: How to solve same transition but diff enterState/leaveState
    // question.
    virtual bool CanTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        if ( fsmContext && fsmContext->mCurStateId == mEnterStateId )
        {
            return true;
        }
        return false;
    }

  private:
};

class SkillTransition_Cast2Spell : public SkillTransition
{
  public:
    SkillTransition_Cast2Spell() : SkillTransition() {}
    SkillTransition_Cast2Spell( SkillStateId enterStateId, SkillStateId leaveStateId )
        : SkillTransition( enterStateId, leaveStateId )
    {
    }
    virtual ~SkillTransition_Cast2Spell() override {}

  public:
    virtual SkillTransitionId GetTransitionId() const override { return SkillTransitionId::Cast2Spell; }

  public:
    virtual void StartTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        EndTransition( fsmContext );
    }

    virtual void EndTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        if ( fsmContext )
        {
            fsmContext->mCurStateId = mLeaveStateId;
        }
    }

  public:
    // TODO: How to solve same transition but diff enterState/leaveState
    // question.
    virtual bool CanTransition( std::shared_ptr<SkillFSMContext> &fsmContext ) override
    {
        if ( fsmContext && fsmContext->mCurStateId == mEnterStateId )
        {
            return true;
        }
        return false;
    }

  private:
};

class SkillFSM : public FSM<SkillState, SkillTransition, SkillFSMContext>
{
  public:
    using Super = FSM<SkillState, SkillTransition, SkillFSMContext>;

  public:
    SkillFSM() : Super() {}
    virtual ~SkillFSM() override {}

  public:
    using Super::GetFSMId;
    static FSMId GetFSMId_S() { return FSMId( STRINGIFY( SKillFSM ) ); }

  public:
    virtual void Build() override
    {
        mStates.push_back( std::make_unique<SkillState_Cast>() );
        mStates.push_back( std::make_unique<SkillState_Spell>() );

        mTransitions.push_back( std::make_unique<SkillTransition_Cast2Spell>( SkillState_Cast::GetStateId_S(),
                                                                              SkillState_Spell::GetStateId_S() ) );
        mTransitions.push_back( std::make_unique<SkillTransition_Spell2Cast>( SkillState_Spell::GetStateId_S(),
                                                                              SkillState_Cast::GetStateId_S() ) );
    }

    virtual bool Exec( std::shared_ptr<SkillFSMContext> &fsmContext ) override { return Super::Exec( fsmContext ); }

  private:
};

class FSMTest : public ::testing::Test
{
  protected:
    FSMTest() {}
    ~FSMTest() override {}

    void SetUp() override {}
    void TearDown() override {}

  protected:
    FSMInstance<SkillFSMContext, SkillFSM> skillFSMInstance;
};

TEST_F( FSMTest, BuildAndExecFSM )
{
    // todo: FSM add log to trace execution path and reflection.
    // todo: add event notify.
    // todo: �Ƿ�Ӧ����ֹһ��Exec��ִ�ж��Transition
    skillFSMInstance.Build();
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Spell );
    skillFSMInstance.Exec();
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Cast );
    skillFSMInstance.Exec(); // execute multiple transition
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Cast );
}