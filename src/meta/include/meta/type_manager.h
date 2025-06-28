#pragma once

#include <core/minimal.h>
#include <meta/minimal.h>

#include "meta/user_type/class.h"
#include "meta/user_type/enum.h"
#include "meta/value/function.h"

struct META_API TypeManager : public Singleton<TypeManager>
{
  public:
    void Add( std::shared_ptr<Enum> e )
    {
        NY_ASSERT( mAllEnums.find( e->GetTypeId() ) == mAllEnums.end() )
        mAllEnums.insert( { e->GetTypeId(), e } );
    }

    std::weak_ptr<Enum> GetEnum( TypeId id ) const
    {
        auto it = mAllEnums.find( id );
        if ( it != mAllEnums.end() )
            return it->second;
        return {};
    }

    void Add( std::shared_ptr<Class> c )
    {
        NY_ASSERT( mAllClasses.find( c->GetTypeId() ) == mAllClasses.end() )
        mAllClasses.insert( { c->GetTypeId(), c } );
    }

    std::weak_ptr<Class> GetClass( TypeId id ) const
    {
        auto it = mAllClasses.find( id );
        if ( it != mAllClasses.end() )
            return it->second;
        return {};
    }

    template <typename T>
        requires std::is_class_v<T>
    std::weak_ptr<Class> GetClass() const
    {
        auto id = TypeIdGenerator::Get<T>();
        return GetClass( id );
    }

    void Add( std::shared_ptr<Function> f )
    {
        NY_ASSERT( mAllFunctions.find( f->GetTypeId() ) == mAllFunctions.end() )
        mAllFunctions.insert( { f->GetTypeId(), f } );
    }

    std::weak_ptr<Function> GetFunction( TypeId id ) const
    {
        auto it = mAllFunctions.find( id );
        if ( it != mAllFunctions.end() )
            return it->second;
        return {};
    }

  protected:
    std::map<TypeId, std::shared_ptr<Enum>> mAllEnums;
    std::map<TypeId, std::shared_ptr<Class>> mAllClasses;

    std::map<TypeId, std::shared_ptr<Function>> mAllFunctions;
};
