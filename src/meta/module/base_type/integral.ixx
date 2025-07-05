module;

#include <module_export.h>

export module meta.base_type.integral;

import core;
import meta.type_base;
import meta.base_type.base_type;

export class META_API Bool : public BaseType
{
  public:
    Bool( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Bool() = default;
};

export class META_API Integer : public BaseType
{
  public:
    Integer( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Integer() = default;
};

export class META_API Unsigned : public Integer
{
  public:
    Unsigned( TypeInfo typeInfo )
        : Integer( typeInfo )
    {
    }
};

export class U8 : public Unsigned
{
  public:
    U8( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

export class U16 : public Unsigned
{
  public:
    U16( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

export class U32 : public Unsigned
{
  public:
    U32( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

export class U64 : public Unsigned
{
  public:
    U64( TypeInfo typeInfo )
        : Unsigned( typeInfo )
    {
    }
};

export class Signed : public Integer
{
  public:
    Signed( TypeInfo typeInfo )
        : Integer( typeInfo )
    {
    }
};

export class I8 : public Signed
{
  public:
    I8( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

export class I16 : public Signed
{
  public:
    I16( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

export class I32 : public Signed
{
  public:
    I32( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};

export class I64 : public Signed
{
  public:
    I64( TypeInfo typeInfo )
        : Signed( typeInfo )
    {
    }
};
