module;

#include <module_export.h>

export module meta.user_type.class_type;

import meta.type_base;
import meta.user_type.struct_type;

export class META_API Class : public Struct
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