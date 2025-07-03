#pragma once

#include "meta/type_base.h"
import core;

class META_API BaseType : public TypeBase
{
  public:
    BaseType( TypeInfo typeInfo )
    {
        UNUSED_VARS( typeInfo );
    }

    virtual ~BaseType() = default;
};
