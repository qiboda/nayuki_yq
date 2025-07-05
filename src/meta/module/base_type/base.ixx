module;

#include <module_export.h>
#include <core/macro/macro.h>

export module meta.base_type.base_type;

import meta.type_base;
import core;

export class META_API BaseType : public TypeBase
{
  public:
    BaseType( TypeInfo typeInfo )
    {
        UNUSED_VARS( typeInfo );
    }

    virtual ~BaseType() = default;
};
