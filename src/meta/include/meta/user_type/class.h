#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>
#include "meta/user_type/struct.h"

class META_API Class : public Struct
{
  public:
    Class()
    {
    }

    Class( TypeInfo typeInfo )
        : Struct( typeInfo )
    {
    }

    virtual ~Class() = default;
};