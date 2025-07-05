module;

#include <module_export.h>
#include <core/macro/macro.h>

export module meta.user_type.property;

import std;
import core;
import meta.type_id;
import meta.user_type.define;

export class META_API IPropertyInfo
{
  public:
    virtual ~IPropertyInfo() = default;

    virtual void* GetPropertyValue( void* StructuredValue ) = 0;

    virtual usize GetPropertyAlign() = 0;

    virtual usize GetPropertySize() = 0;

    virtual void* GetPropertyAddress() = 0;
};

export template <auto Ptr>
struct PropertyPtrTrait : std::false_type, public IPropertyInfo
{
    virtual void* GetPropertyValue( void* StructuredValue ) override
    {
        UNUSED_VARS( StructuredValue );
        return nullptr;
    };

    virtual usize GetPropertyAlign() override
    {
        return 0;
    }

    virtual usize GetPropertySize() override
    {
        return 0;
    }

    virtual void* GetPropertyAddress() override
    {
        return nullptr;
    }
};

export template <typename TClass, typename TProperty, TProperty TClass::* Ptr>
struct PropertyPtrTrait<Ptr> : std::true_type, public IPropertyInfo
{
    using Type = TProperty TClass::*;
    using ClassType = TClass;
    using PropertyType = TProperty;

    inline static Type PropertyPtr = Ptr;

    PropertyType* GetPropertyValueInner( ClassType* c )
    {
        // std::invoke( Ptr, c );
        return c->*PropertyPtr;
    }

  public:
    virtual void* GetPropertyValue( void* StructuredValue ) override
    {
        auto* structuredValue = static_cast<ClassType*>( StructuredValue );
        return GetPropertyValueInner( structuredValue );
    }

    virtual usize GetPropertyAlign() override
    {
        return alignof( TProperty );
    }

    virtual usize GetPropertySize() override
    {
        return sizeof( TProperty );
    }

    virtual void* GetPropertyAddress() override
    {
        return static_cast<void*>( &PropertyPtr );
    }
};

export template <auto Ptr>
inline bool IsPropertyPtrValue = PropertyPtrTrait<Ptr>::value;

export template <auto Ptr>
concept IsPropertyPtrConcept = IsPropertyPtrValue<Ptr>;

/**
 * @brief 非静态成员变量
 */
export class META_API Property
{
  public:
    Property();

    ~Property()
    {
        if ( mPropertyInfo )
        {
            delete mPropertyInfo;
        }
    }

    template <auto Ptr>
    void Create( std::string& name, TypeId typeId )
    {
        mName = name;
        mTypeId = typeId;
        mPropertyInfo = new PropertyPtrTrait<Ptr>();
    }

    void SetAccessLevel( AccessLevel accessLevel )
    {
        mAccessLevel = accessLevel;
    }

    template <typename T>
    T* GetValue( void* StructuredValue )
    {
        return reinterpret_cast<T*>( mPropertyInfo->GetPropertyValue( StructuredValue ) );
    }

  protected:
    IPropertyInfo* mPropertyInfo = nullptr;

    // 限定符
    QualifierFlag mQualifierFlag = QualifierFlag::None;
    // 复合类型
    CompoundFlag mCompoundFlag = CompoundFlag::None;

    // 属性的类型ID
    // 包括限定符和复合类型
    TypeId mTypeId;
    // 属性的名字
    std::string mName;

    // 访问级别
    AccessLevel mAccessLevel = AccessLevel::Public;
};