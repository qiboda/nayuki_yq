#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>
#include "meta/user_type/structured.h"

class META_API Class : public Structured
{
  public:
    Class( TypeInfo typeInfo )
        : Structured( typeInfo )
    {
    }

    virtual ~Class() = default;

  public:
    template <typename TFunc>
    void AddFunc( std::string& name )
    {
        UNUSED_VAR( name );
    }
};