module;

#include <module_export.h>

export module meta.base_type.float_type;

import meta.base_type.base_type;
import meta.type_base;
import core;

export class META_API Float : public BaseType
{
  public:
    Float( TypeInfo typeInfo )
        : BaseType( typeInfo )
    {
    }

    virtual ~Float() = default;
};

export class META_API F32 : public Float
{
  public:
    F32( TypeInfo typeInfo )
        : Float( typeInfo )
    {
    }
};

export class META_API F64 : public Float
{
  public:
    F64( TypeInfo typeInfo )
        : Float( typeInfo )
    {
    }
};
