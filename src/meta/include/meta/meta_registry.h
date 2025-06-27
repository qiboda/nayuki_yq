#pragma once

#include "meta/user_type/class.h"
#include <core/minimal.h>
#include <meta/minimal.h>

class META_API ClassRegistry
{
  public:
    ClassRegistry();

  public:
    template <typename T>
        requires std::is_class_v<T>
    ClassRegistry& Create( std::string& name )
    {
        auto typeInfo = TypeInfo( TypeIdGenerator::Next(), name );
        c = new Class( typeInfo );
        return *this;
    }

    template <typename T>
    ClassRegistry& AddMethod( T func, std::string& name, AccessLevel accessLevel = AccessLevel::Public )
    {
        Method method;
        method.Create( func, name );
        method.SetAccessLevel( accessLevel );
        c->AddMethod( method );
        return *this;
    }

    template <auto Ptr>
    void AddProperty( std::string& name, AccessLevel accessLevel = AccessLevel::Public )
    {
        PropertyMemoryInfo memoryInfo;
        memoryInfo.mData = PropertyPtrTrait<Ptr>::PropertyPtr;
        memoryInfo.mSize = PropertyPtrTrait<Ptr>::PropertySize;
        memoryInfo.mAlignment = PropertyPtrTrait<Ptr>::PropertyAlign;

        // TODO: 查询Id。
        Property property;
        property.Create( name, TypeIdGenerator::Next() );
        property.SetAccessLevel( accessLevel ); // 默认公共访问级别
        property.SetMemoryInfo<Ptr>();
        c->AddProperty( property );
    }

  protected:
    Class* c = nullptr;
};

class META_API MetaRegistry
{
  public:
    MetaRegistry();

  public:
    template <typename T>
        requires std::is_class_v<T>
    void Class()
    {
    }

    void Struct()
    {
    }

    void Enum()
    {
    }

    void Func()
    {
    }

    void Value()
    {
    }
};