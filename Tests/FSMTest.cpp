#include <gtest/gtest.h>

#include <NayukiYq/NayukiYq.h>

#include <NayukiYq/Common/FSM/FSM.h>
#include <NayukiYq/Common/FSM/FSMContext.h>
#include <NayukiYq/Common/FSM/FSMInstance.h>

#include <NayukiYq/Common/FSM/State.h>

#include <NayukiYq/Common/FSM/Transition.h>

enum class SkillStateId
: ui8
{
    Spell,
    Cast,
    Max,
};

class SkillFSMContext
    : public FSMContext
{
public:
    SkillFSMContext()
        : FSMContext() {}
    virtual ~SkillFSMContext() {}

public:
    SkillStateId mCurStateId = SkillStateId::Spell;
};

class SkillState
    : public State<SkillStateId, SkillFSMContext>
{
public:
    SkillState() {}
    virtual ~SkillState() {}

private:
};

class SkillState_Spell
    : public SkillState
{
public:
    SkillState_Spell()
        : SkillState() {}
    virtual ~SkillState_Spell() {}

public:
    using SkillState::GetStateId;
    static SkillStateId GetStateId_S() { return SkillStateId::Spell; }

public:
    virtual void OnUpdate( std::shared_ptr<SkillFSMContext>& fsmContext, f32 DeltaSeconds ) override
    {
        UNUSED_VAR( fsmContext );
        UNUSED_VAR( DeltaSeconds );
    }

    virtual void OnEnterPre( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnter( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnterPost( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePre( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeave( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePost( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }

private:
};

class SkillState_Cast
    : public SkillState
{
public:
    SkillState_Cast()
        : SkillState() {}
    virtual ~SkillState_Cast() {}

public:
    using SkillState::GetStateId;
    static SkillStateId GetStateId_S() { return SkillStateId::Cast; }

public:
    virtual void OnUpdate( std::shared_ptr<SkillFSMContext>& fsmContext, f32 DeltaSeconds ) override
    {
        UNUSED_VAR( fsmContext );
        UNUSED_VAR( DeltaSeconds );
    }

    virtual void OnEnterPre( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnter( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnEnterPost( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePre( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeave( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }
    virtual void OnLeavePost( std::shared_ptr<SkillFSMContext>& fsmContext ) override { UNUSED_VAR( fsmContext ); }

private:
};

enum class SkillTransitionId
: ui8
{
    Spell2Cast,
    Cast2Spell,
    Max,
};

class SkillTransition
    : public Transition<SkillTransitionId, SkillStateId, SkillFSMContext>
{
public:
    using Super = Transition<SkillTransitionId, SkillStateId, SkillFSMContext>;

public:
    SkillTransition()
        : Super() {}
    SkillTransition( SkillStateId enterStateId, SkillStateId leaveStateId )
        : Super( enterStateId, leaveStateId ) {}
    virtual ~SkillTransition() {}
};

class SkillTransition_Spell2Cast
    : public SkillTransition
{
public:
    SkillTransition_Spell2Cast()
        : SkillTransition() {}
    SkillTransition_Spell2Cast( SkillStateId enterStateId, SkillStateId leaveStateId )
        : SkillTransition( enterStateId, leaveStateId ) {}
    virtual ~SkillTransition_Spell2Cast() {}

public:
    virtual SkillTransitionId GetTransitionId() const override { return SkillTransitionId::Spell2Cast; }

public:
    virtual void StartTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        EndTransition( fsmContext );
    }

    virtual void EndTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        if ( fsmContext )
        {
            fsmContext->mCurStateId = mLeaveStateId;
        }
    }

public:
    // TODO: How to solve same transition but diff enterState/leaveState question.
    virtual bool CanTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        if ( fsmContext && fsmContext->mCurStateId == mEnterStateId )
        {
            return true;
        }
        return false;
    }

private:
};

class SkillTransition_Cast2Spell
    : public SkillTransition
{
public:
    SkillTransition_Cast2Spell()
        : SkillTransition() {}
    SkillTransition_Cast2Spell( SkillStateId enterStateId, SkillStateId leaveStateId )
        : SkillTransition( enterStateId, leaveStateId ) {}
    virtual ~SkillTransition_Cast2Spell() {}

public:
    virtual SkillTransitionId GetTransitionId() const override { return SkillTransitionId::Cast2Spell; }

public:
    virtual void StartTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        EndTransition( fsmContext );
    }

    virtual void EndTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        if ( fsmContext )
        {
            fsmContext->mCurStateId = mLeaveStateId;
        }
    }

public:
    // TODO: How to solve same transition but diff enterState/leaveState question.
    virtual bool CanTransition( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        if ( fsmContext && fsmContext->mCurStateId == mEnterStateId )
        {
            return true;
        }
        return false;
    }

private:
};

class SkillFSM
    : public FSM<SkillState, SkillTransition, SkillFSMContext>
{
public:
    using Super = FSM<SkillState, SkillTransition, SkillFSMContext>;

public:
    SkillFSM()
        : Super() {}
    virtual ~SkillFSM() {}

public:
    using Super::GetFSMId;
    static FSMId GetFSMId_S() { return FSMId( STRINGIFY( SKillFSM ) ); }

public:
    virtual void Build() override
    {
        mStates.push_back( std::make_unique<SkillState_Cast>() );
        mStates.push_back( std::make_unique<SkillState_Spell>() );

        mTransitions.push_back(
            std::make_unique<SkillTransition_Cast2Spell>(
                SkillState_Cast::GetStateId_S(),
                SkillState_Spell::GetStateId_S()
            )
        );
        mTransitions.push_back(
            std::make_unique<SkillTransition_Spell2Cast>(
                SkillState_Spell::GetStateId_S(),
                SkillState_Cast::GetStateId_S()
            )
        );
    }

    virtual bool Exec( std::shared_ptr<SkillFSMContext>& fsmContext ) override
    {
        return Super::Exec( fsmContext );
    }

private:
};

class FSMTest
    : public ::testing::Test
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
    // todo: 是否应该阻止一次Exec，执行多个Transition
    skillFSMInstance.Build();
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Spell );
    skillFSMInstance.Exec();
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Cast );
    skillFSMInstance.Exec(); // execute multiple transition
    ASSERT_EQ( skillFSMInstance.GetFSMContext().lock()->mCurStateId, SkillStateId::Cast );
}