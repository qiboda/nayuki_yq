#pragma once

#include "meta/type_base.h"
#include "meta/user_type/namespace.h"
#include <core/minimal.h>
#include <meta/minimal.h>

enum class EnumUnderlyingType : u8
{
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
};

template <typename T>
struct EnumUnderlyingTypeTrait;

template <>
struct EnumUnderlyingTypeTrait<i8>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::I8;
};

template <>
struct EnumUnderlyingTypeTrait<i16>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::I8;
};

template <>
struct EnumUnderlyingTypeTrait<i32>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::I32;
};

template <>
struct EnumUnderlyingTypeTrait<i64>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::I64;
};

template <>
struct EnumUnderlyingTypeTrait<u8>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::U8;
};

template <>
struct EnumUnderlyingTypeTrait<u16>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::U16;
};

template <>
struct EnumUnderlyingTypeTrait<u32>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::U32;
};

template <>
struct EnumUnderlyingTypeTrait<u64>
{
    inline static constexpr EnumUnderlyingType Value = EnumUnderlyingType::U64;
};

template <typename T>
inline constexpr EnumUnderlyingType EnumUnderlyingTypeValue = EnumUnderlyingTypeTrait<T>::Value;

template <>
struct fmt::formatter<EnumUnderlyingType> : fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format( EnumUnderlyingType e, FormatContext& ctx ) const
    {
        switch ( e )
        {
        case EnumUnderlyingType::I8:
            return fmt::formatter<std::string>::format( "i8", ctx );
        case EnumUnderlyingType::I16:
            return fmt::formatter<std::string>::format( "i16", ctx );
        case EnumUnderlyingType::I32:
            return fmt::formatter<std::string>::format( "i32", ctx );
        case EnumUnderlyingType::I64:
            return fmt::formatter<std::string>::format( "i64", ctx );
        case EnumUnderlyingType::U8:
            return fmt::formatter<std::string>::format( "u8", ctx );
        case EnumUnderlyingType::U16:
            return fmt::formatter<std::string>::format( "u16", ctx );
        case EnumUnderlyingType::U32:
            return fmt::formatter<std::string>::format( "u32", ctx );
        case EnumUnderlyingType::U64:
            return fmt::formatter<std::string>::format( "u64", ctx );
        }
        return fmt::formatter<std::string>::format( "unknown", ctx );
    }
};

struct EnumField
{
    std::string mName;
    u64 mValue;
};

class META_API Enum : public TypeBase
{
  public:
    explicit Enum( TypeInfo typeInfo,
                   EnumUnderlyingType underlyingType = EnumUnderlyingType::U8,
                   std::unique_ptr<Namespace> mNamespace = nullptr )
        : TypeBase( typeInfo )
        , mUnderlyingType( underlyingType )
        , mNamespace( std::move( mNamespace ) )
    {
    }

  public:
    template <typename T>
        requires std::is_integral_v<T>
    void Add( std::string name, T value )
    {
        mAllFields.push_back( { name, reinterpret_cast<u64>( value ) } );
    }

    void Insert( u32 index, std::string name, u64 value )
    {
        if ( index < mAllFields.size() )
        {
            // 元素少，不需要考虑数据后移问题
            mAllFields.insert( mAllFields.begin() + index, { name, value } );
        }
        else
        {
            mAllFields.push_back( { name, value } );
        }
    }

    template <typename T>
    T* GetValue( std::string name ) const
    {
        for ( const auto& field : mAllFields )
        {
            if ( field.mName == name )
            {
                return &reinterpret_cast<T>( field.mValue );
            }
        }
        return nullptr;
    }

    EnumField* GetField( u32 index )
    {
        if ( index < mAllFields.size() )
        {
            return &mAllFields[index];
        }
        return nullptr;
    }

    auto begin()
    {
        return mAllFields.begin();
    }

    auto end()
    {
        return mAllFields.end();
    }

    auto size() const
    {
        return mAllFields.size();
    }

    EnumUnderlyingType GetUnderlyingType() const
    {
        return mUnderlyingType;
    }

    std::string GetUnderlyingTypeName() const
    {
        return fmt::format( "{}", mUnderlyingType );
    }

  protected:
    std::vector<EnumField> mAllFields;

    EnumUnderlyingType mUnderlyingType;

    std::unique_ptr<Namespace> mNamespace = nullptr;
};
