#pragma once

#include <core/minimal.h>
#include <meta/minimal.h>

#include "meta/user_type/structured.h"

class META_API Struct : public Structured
{
  public:
    Struct( TypeInfo typeInfo )
        : Structured( typeInfo )
    {
    }
};