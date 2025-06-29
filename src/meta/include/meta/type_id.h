#pragma once

#include "core/registry/id.h"
#include <module_export.h>

struct META_API TypeId : public Id
{
    TypeId() = default;
};

struct META_API TypeIdGenerator : public IdGenerator<TypeId>
{
    template <typename TType>
    static TypeId Get()
    {
        static const TypeId id = Next();
        return id;
    }

    template <typename TType>
        requires std::is_const_v<TType> || std::is_volatile_v<TType> || std::is_reference_v<TType>
    static TypeId Get()
    {
        return Get<std::remove_cvref_t<TType>>();
    }
};
