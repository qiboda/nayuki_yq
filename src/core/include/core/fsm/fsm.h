#pragma once

#include <core/misc/non_copyable.h>
#include <core/macro/macro.h>

#include <core/fsm/fsm_context.h>
#include <core/fsm/fsm_interface.h>
#include <core/fsm/state.h>
#include <core/fsm/transition.h>

#include <vector>

template <typename TState, typename TTransition, typename TFSMContext>
class FSM : public FSMInterface, public NonCopyable
{
  public:
    FSM()
    {
    }

    virtual ~FSM() override
    {
    }

  public:
    static FSMId GetFSMId_S()
    {
        return FSMId( STRINGIFY( FSM ) );
    }

    virtual FSMId GetFSMId() const override
    {
        return GetFSMId_S();
    }

  public:
    virtual bool Exec( std::shared_ptr<TFSMContext>& fsmContext )
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
