#pragma once

#include "core/macro/macro.h"
#include "meta/user_type/class.h"
#include <core/minimal.h>
#include <meta/minimal.h>
#include <meta/type_manager.h>

class META_API ClassRegistry
{
  public:
    ClassRegistry()
    {
    }

  public:
    template <typename T>
        requires std::is_class_v<T>
    ClassRegistry& Create( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        auto typeId = TypeIdGenerator::Get<T>();
        auto typeInfo = TypeInfo( typeId, name );
        c = std::make_shared<Class>( typeInfo );
        c->SetNamespace( std::move( ns ) );
        TypeManager::GetInstance().Add( c );
        return *this;
    }

    template <auto FuncPtr>
    ClassRegistry& AddMethod( const std::string& name, AccessLevel accessLevel = AccessLevel::Public )
    {
        auto method = std::make_shared<Method>();
        method->Create<FuncPtr>( name, accessLevel );
        c->AddMethod( method );
        return *this;
    }

    template <auto FuncPtr>
    ClassRegistry& AddFunction( const std::string& name, AccessLevel accessLevel = AccessLevel::Public )
    {
        auto function = std::make_shared<Function>();
        function->Create<FuncPtr>( name );
        function->SetAccessLevel( accessLevel ); // 默认公共访问级别
        c->AddFunction( function );
        return *this;
    }

    template <auto MemberPtr>
    ClassRegistry& AddProperty( const std::string& name, AccessLevel accessLevel = AccessLevel::Public )
    {
        using PropertyType = PropertyPtrTrait<MemberPtr>::PropertyType;
        TypeId typeId = TypeIdGenerator::Get<PropertyType>();

        auto property = std::make_shared<Property>();
        property->Create<MemberPtr>( name, typeId );
        property->SetAccessLevel( accessLevel ); // 默认公共访问级别
        c->AddProperty( property );

        return *this;
    }

  protected:
    std::shared_ptr<Class> c = nullptr;
};

class META_API EnumRegistry
{
  public:
    EnumRegistry()
    {
    }

  public:
    template <typename T>
        requires std::is_enum_v<T>
    EnumRegistry& Create( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        auto typeId = TypeIdGenerator::Get<T>();
        auto typeInfo = TypeInfo( typeId, name );
        e = std::make_shared<Enum>( typeInfo, EnumUnderlyingTypeValue<std::underlying_type_t<T>>, std::move( ns ) );
        TypeManager::GetInstance().Add( e );
        return *this;
    }

    template <typename T>
        requires std::is_integral_v<T>
    EnumRegistry& AddField( const std::string& name, T value )
    {
        NY_ASSERT_MSG( e, "Enum must be created before adding fields." );
        e->Add( name, value );
        return *this;
    }

  protected:
    std::shared_ptr<Enum> e = nullptr;
};

class META_API FunctionRegistry
{
  public:
    FunctionRegistry()
    {
    }

  public:
    template <auto FuncPtr>
    FunctionRegistry& Create( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        f = std::make_shared<Function>();
        f->Create<FuncPtr>( name, std::move( ns ) );
        TypeManager::GetInstance().Add( f );
        return *this;
    }

  protected:
    std::shared_ptr<Function> f = nullptr;
};

class META_API MetaRegistry
{
  public:
    template <typename T>
        requires std::is_class_v<T>
    static ClassRegistry Class( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        return ClassRegistry().Create<T>( name, std::move( ns ) );
    }

    template <typename T>
        requires std::is_class_v<T>
    static EnumRegistry Enum( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        return EnumRegistry().Create<T>( name, std::move( ns ) );
    }

    /**
     * @brief 全局函数
     */
    template <auto FuncPtr>
    static FunctionRegistry& Func( const std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        return FunctionRegistry().Create<FuncPtr>( name, std::move( ns ) );
    }

    static void Value()
    {
    }
};