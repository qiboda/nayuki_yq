#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>

class META_API Structured : public TypeBase
{
  public:
    Structured( TypeInfo typeInfo )
        : TypeBase( typeInfo )
    {
    }

    virtual ~Structured() = default;

  public:
    template <typename TFunc>
    void AddFunc( std::string& name )
    {
        UNUSED_VAR( name );
    }

  protected:
    // properties and values and methods and functions
};
