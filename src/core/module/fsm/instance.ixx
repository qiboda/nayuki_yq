export module core.fsm.instance;

import core.fsm.builder;
import core.fsm.interface;

import core.misc.non_copyable;
import std;

export template <typename TFSMContext, typename TFSM>
class FSMInstance : public NonCopyable
{
  public:
    void Build()
    {
        if ( mFSM.expired() )
        {
            auto fsmInterface = FSMBuilder::GetInstance().FindOrCreateFSM<TFSM>();
            if ( fsmInterface.expired() == false )
            {
                mFSM = std::dynamic_pointer_cast<TFSM, FSMInterface>( fsmInterface.lock() );
            }
        }

        if ( mFSMContext == nullptr )
        {
            mFSMContext = std::make_shared<TFSMContext>();
        }
    }

  public:
    bool Exec()
    {
        if ( mFSM.expired() == false )
        {
            return mFSM.lock()->Exec( mFSMContext );
        }
        return false;
    }

  public:
    std::weak_ptr<TFSMContext> GetFSMContext()
    {
        return mFSMContext;
    }

    std::weak_ptr<TFSM> GetFSM()
    {
        return mFSM;
    }

  private:
    std::shared_ptr<TFSMContext> mFSMContext;
    std::weak_ptr<TFSM> mFSM;
};
