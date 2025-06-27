#pragma once

#include <core/minimal.h>
#include <meta/minimal.h>

#include "meta/user_type/class.h"
#include "meta/user_type/enum.h"
#include "meta/user_type/struct.h"
#include "meta/value/function.h"
#include "meta/value/value.h"

struct TypeIdRegistry
{
  public:
    void Add( Enum* e )
    {
        NY_ASSERT( mAllEnums.find( e->GetTypeId() ) == mAllEnums.end() )
        mAllEnums.insert( { e->GetTypeId(), e } );
    }

    Enum* GetEnum( TypeId id ) const
    {
        auto it = mAllEnums.find( id );
        if ( it != mAllEnums.end() )
            return it->second;
        return nullptr;
    }

    void Add( Class* c )
    {
        NY_ASSERT( mAllClasses.find( c->GetTypeId() ) == mAllClasses.end() )
        mAllClasses.insert( { c->GetTypeId(), c } );
    }

    Class* GetClass( TypeId id ) const
    {
        auto it = mAllClasses.find( id );
        if ( it != mAllClasses.end() )
            return it->second;
        return nullptr;
    }

    void Add( Struct* s )
    {
        NY_ASSERT( mAllStructs.find( s->GetTypeId() ) == mAllStructs.end() )
        mAllStructs.insert( { s->GetTypeId(), s } );
    }

    Struct* GetStruct( TypeId id ) const
    {
        auto it = mAllStructs.find( id );
        if ( it != mAllStructs.end() )
            return it->second;
        return nullptr;
    }

  protected:
    std::map<TypeId, Enum*> mAllEnums;
    std::map<TypeId, Class*> mAllClasses;
    std::map<TypeId, Struct*> mAllStructs;
    // todo: 也需要存储其他类型，包括各种复合类型。
};

struct ValueIdRegistry
{
    std::map<TypeId, Function*> mAllFunctions;
    std::map<TypeId, Value*> mAllValues;
};