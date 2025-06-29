#pragma once

#include "meta/type_base.h"
#include <core/core.h>

class META_API BaseType : public TypeBase
{
  public:
    BaseType( TypeInfo typeInfo )
    {
        UNUSED_VARS( typeInfo );
    }

    virtual ~BaseType() = default;
};
