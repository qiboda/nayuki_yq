#pragma once

#include "meta/type_id.h"
#include <core/minimal.h>
#include <meta/minimal.h>

enum class TypeKind : u8
{
    None,
    // 基本类型
    Base,
    Class,
    Struct,
    Enum,
};

struct TypeInfo
{
    TypeId mId;
    std::string mName;

    TypeInfo()
    {
    }

    TypeInfo( TypeId id, const std::string& name )
        : mId( id )
        , mName( name )
    {
    }
};

class META_API TypeBase
{
  public:
    TypeBase()
    {
    }

    TypeBase( TypeInfo typeInfo );
    virtual ~TypeBase() = default;

    void SetTypeInfo( TypeInfo typeInfo )
    {
        mTypeInfo = typeInfo;
    }

    TypeId GetTypeId()
    {
        return mTypeInfo.mId;
    }

  protected:
    TypeInfo mTypeInfo;
};
