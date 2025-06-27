#pragma once

#include "meta/type_base.h"
#include "meta/user_type/define.h"
#include "meta/user_type/property.h"
#include "meta/value/method.h"
#include <core/minimal.h>
#include <meta/minimal.h>

struct META_API InheritRelative
{
    // 继承的类型ID
    TypeId mTypeId;
    // 继承的访问级别
    AccessLevel mAccessLevel;
    // 是否是虚继承
    bool bVirtual = false;
};

/**
 * @brief
 * 继承关系。
 * 类或者结构体的成员变量。
 * 成员方法。
 */
class META_API Structured : public TypeBase
{
  public:
    Structured()
    {
    }

    Structured( TypeInfo typeInfo )
        : TypeBase( typeInfo )
    {
    }

    virtual ~Structured() = default;

  public:
    void AddMethod( Method method )
    {
        mMethods.push_back( method );
    }

    void AddProperty( Property property )
    {
        mProperties.push_back( property );
    }

  protected:
    // 对齐方式
    u32 mAlignment;
    // 尺寸
    u32 mSize;

  protected:
    // 成员变量
    std::vector<Property> mProperties;
    // 成员方法
    std::vector<Method> mMethods;

  protected:
    // 继承关系
    std::vector<InheritRelative> mInheritRelatives;
};
