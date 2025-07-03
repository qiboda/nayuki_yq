#pragma once

#include "meta/base_type/base.h"
#include "meta/type_base.h"

class META_API Bool : public BaseType
{
  public:
    Bool( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Bool() = default;
};

class META_API Integer : public BaseType
{
  public:
    Integer( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Integer() = default;
};

class META_API Unsigned : public Integer
{
  public:
    Unsigned( TypeInfo typeInfo )
        : Integer( typeInfo )
    {
    }
};

class U8 : public Unsigned
{
  public:
    U8( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

class U16 : public Unsigned
{
  public:
    U16( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

class U32 : public Unsigned
{
  public:
    U32( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

class U64 : public Unsigned
{
  public:
    U64( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

class Signed : public Integer
{
  public:
    Signed( TypeInfo typeInfo )
        : Integer( typeInfo )
    {
    }
};

class I8 : public Signed
{
  public:
    I8( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

class I16 : public Signed
{
  public:
    I16( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

class I32 : public Signed
{
  public:
    I32( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

class I64 : public Signed
{
  public:
    I64( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};
