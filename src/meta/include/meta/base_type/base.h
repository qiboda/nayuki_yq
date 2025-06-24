#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>

class META_API BaseType : public TypeBase
{
  public:
    BaseType( TypeInfo typeInfo );
    virtual ~BaseType() = default;
};
