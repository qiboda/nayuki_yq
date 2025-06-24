#pragma once

#include "core/registry/id.h"
#include <core/minimal.h>
#include <meta/minimal.h>

struct META_API TypeId : public Id
{
};

struct TypeIdGenerator : public IdGenerator<TypeId>
{
};

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
    TypeBase( TypeInfo typeInfo );
    virtual ~TypeBase() = default;

    TypeId GetTypeId()
    {
        return mTypeInfo.mId;
    }

  protected:
    TypeInfo mTypeInfo;
};
