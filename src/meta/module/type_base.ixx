module;

#include <module_export.h>

export module meta.type_base;

import std;
import core;
import meta.type_id;

export enum class TypeKind : u8 {
    None,
    // 基本类型
    Base,
    Class,
    Struct,
    Enum,
};

export struct META_API TypeInfo
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

export class META_API TypeBase
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
