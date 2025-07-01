module;

#include <module_export.h>

export module core.fsm:interface;

import std;

export struct CORE_API FSMId
{
    FSMId()
        : id()
    {
    }

    FSMId( const std::string& id )
        : id( id )
    {
    }

    bool operator==( const FSMId& fsmId ) const
    {
        return fsmId.id == id;
    }

    bool operator!=( const FSMId& fsmId ) const
    {
        return !( *this == fsmId );
    }

    std::string id;
};

static const FSMId NullFSMId;

export class CORE_API FSMInterface
{

  public:
    FSMInterface() = default;
    virtual ~FSMInterface() = default;

    FSMInterface( const FSMInterface& ) = default;
    FSMInterface& operator=( const FSMInterface& ) = default;

  public:
    template <typename T>
    static T Cast( FSMInterface* fsmInterface )
    {
        if ( fsmInterface && fsmInterface->GetFSMId() == T::GetFSMId_S() )
        {
            return static_cast<T>( fsmInterface );
        }
    }

  public:
    static FSMId GetFSMId_S()
    {
        return NullFSMId;
    }

    virtual FSMId GetFSMId() const
    {
        return GetFSMId_S();
    }

  public:
    virtual void Build() = 0;
};
