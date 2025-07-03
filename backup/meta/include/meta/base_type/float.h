#pragma once

#include "meta/base_type/base.h"

class META_API Float : public BaseType
{
  public:
    Float( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Float() = default;
};

class META_API F32 : public Float
{
  public:
    F32( TypeInfo typeInfo )
        : Float( typeInfo )
    {
    }
};

class META_API F64 : public Float
{
  public:
    F64( TypeInfo typeInfo )
        : Float( typeInfo )
    {
    }
};
