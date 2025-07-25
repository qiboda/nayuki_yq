module;

#include <module_export.h>

export module meta.user_type.struct_type;

import std;
import core;
import meta.type_id;
import meta.type_base;
import meta.user_type.ns;
import meta.user_type.property;
import meta.value.method;
import meta.value.function;
import meta.user_type.define;

/**
 * @brief 禁止虚继承(菱形继承)
 */
export struct META_API InheritRelative
{
    // 继承的类型ID
    TypeId mTypeId;
    // 继承的访问级别
    AccessLevel mAccessLevel;
    // 是否是虚继承
    // bool bVirtual = false;
};

/**
 * @brief
 * 继承关系。
 * 类或者结构体的成员变量。
 * 成员方法。
 */
export class META_API Struct : public TypeBase
{
  public:
    Struct()
    {
    }

    Struct( TypeInfo typeInfo )
        : TypeBase( typeInfo )
    {
    }

    virtual ~Struct() = default;

  public:
    void AddMethod( std::shared_ptr<Method> method )
    {
        mMethods.push_back( method );
    }

    void AddFunction( std::shared_ptr<Function> function )
    {
        mFunctions.push_back( function );
    }

    void AddProperty( std::shared_ptr<Property> property )
    {
        mProperties.push_back( property );
    }

    void SetNamespace( std::unique_ptr<Namespace> ns )
    {
        mNamespace = std::move( ns );
    }

  protected:
    // 对齐方式
    usize mAlignment;
    // 尺寸
    usize mSize;

  protected:
    // 成员变量
    std::vector<std::shared_ptr<Property>> mProperties;
    // 成员方法
    std::vector<std::shared_ptr<Method>> mMethods;
    // 静态方法
    std::vector<std::shared_ptr<Function>> mFunctions;

  protected:
    // 继承关系
    std::vector<InheritRelative> mInheritRelatives;

    // 命名空间
    std::unique_ptr<Namespace> mNamespace = nullptr;
};
