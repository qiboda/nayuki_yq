#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>

/**
 * @brief T*, T&, T&&, T[]等复合类型。
 * TypeBase 内存储复合类型的信息。
 * 原始类型的信息存储在原始类型中
 */
class META_API Compound : public TypeBase
{
  public:
    Compound( TypeInfo typeInfo )
        : TypeBase( typeInfo )
    {
    }

    virtual ~Compound() = default;
};

class META_API Pointer : public Compound
{
  public:
    Pointer( TypeInfo typeInfo )
        : Compound( typeInfo )
    {
    }

    virtual ~Pointer() = default;
};

class META_API LValueRef : public Compound
{
  public:
    LValueRef( TypeInfo typeInfo )
        : Compound( typeInfo )
    {
    }

    virtual ~LValueRef() = default;
};

class META_API RValueRef : public Compound
{
  public:
    RValueRef( TypeInfo typeInfo )
        : Compound( typeInfo )
    {
    }

    virtual ~RValueRef() = default;
};

class META_API RawArray : public Compound
{
  public:
    RawArray( TypeInfo typeInfo )
        : Compound( typeInfo )
    {
    }

    virtual ~RawArray() = default;

  public:
    // 数组的大小
    u32 mSize = 0;
};