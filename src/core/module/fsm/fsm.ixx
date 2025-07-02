module;

#include <core/macro/macro.h>

export module core.fsm:fsm;

import :interface;
import :context;
import :state;
import :transition;

import std;
import core.misc;
import core;

export template <typename TState, typename TTransition, typename TFSMContext>
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
        for ( usize i = 0u; i < mTransitions.size(); ++i )
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
