#pragma once

#include "meta/type_base.h"

class META_API BaseType : public TypeBase
{
  public:
    BaseType( TypeInfo typeInfo );
    virtual ~BaseType() = default;
};
