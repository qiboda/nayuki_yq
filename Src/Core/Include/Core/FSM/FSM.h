#pragma once

#include <Core/Misc/NonCopyable.h>

#include <Core/FSM/FSMContext.h>
#include <Core/FSM/FSMInterface.h>
#include <Core/FSM/State.h>
#include <Core/FSM/Transition.h>

#include <vector>

template <typename TState, typename TTransition, typename TFSMContext>
class FSM : public FSMInterface, public NonCopyable
{
  public:
    FSM() {}
    virtual ~FSM() {}

  public:
    static FSMId GetFSMId_S() { return FSMId( STRINGIFY( FSM ) ); }
    virtual FSMId GetFSMId() const { return GetFSMId_S(); }

  public:
    virtual bool Exec( std::shared_ptr<TFSMContext> &fsmContext )
    {
        for ( size_t i = 0u; i < mTransitions.size(); ++i )
        {
            if ( mTransitions.at( i ) )
            {
                if ( mTransitions.at( i )->CanTransition( fsmContext ) )
                {
                    mTransitions.at( i )->StartTransition( fsmContext );
                }
            }
        }
        return false;
    }

  protected:
    std::vector<std::unique_ptr<TState>> mStates;
    std::vector<std::unique_ptr<TTransition>> mTransitions;
};
