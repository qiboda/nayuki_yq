#pragma once

#include "core/fsm/fsm_interface.h"
#include "core/misc/singleton.h"

#include <core/fsm/fsm.h>
#include <algorithm>

class CORE_API FSMBuilder : public Singleton<FSMBuilder>
{
  public:
    template <typename TFSM>
    std::weak_ptr<FSMInterface> FindOrCreateFSM()
    {
        std::weak_ptr<FSMInterface> fsmInterface = FindFSM( TFSM::GetFSMId_S() );
        if ( fsmInterface.expired() == false )
        {
            return fsmInterface;
        }

        fsmInterface = CreateFSM<TFSM>();
        if ( fsmInterface.expired() == false )
        {
            return fsmInterface;
        }

        return std::weak_ptr<FSMInterface>();
    }

  private:
    std::weak_ptr<FSMInterface> FindFSM( FSMId fsmId )
    {
        auto it = std::find_if( mFSMs.begin(),
                                mFSMs.end(),
                                [fsmId]( const std::shared_ptr<FSMInterface> fsmInterface )
                                {
                                    if ( fsmInterface )
                                    {
                                        return fsmInterface->GetFSMId() == fsmId;
                                    }
                                    return false;
                                } );
        if ( it == mFSMs.end() )
        {
            return std::weak_ptr<FSMInterface>();
        }
        return *it;
    }

    template <typename TFSM>
    std::weak_ptr<FSMInterface> CreateFSM()
    {
        std::shared_ptr<FSMInterface> fsmInterface = std::make_shared<TFSM>();
        if ( fsmInterface->GetFSMId() != TFSM::GetFSMId_S() )
        {
            // TODO: warning log
        }

        fsmInterface->Build();
        mFSMs.push_back( fsmInterface );
        return fsmInterface;
    }

  private:
    std::vector<std::shared_ptr<FSMInterface>> mFSMs;
};
