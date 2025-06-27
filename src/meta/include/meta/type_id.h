#pragma once

#include <core/minimal.h>
#include <meta/minimal.h>
#include "core/registry/id.h"

struct META_API TypeId : public Id
{
};

struct TypeIdGenerator : public IdGenerator<TypeId>
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
