module;

#include "module_export.h"

export module ecs:entity;

import :component;

import std;
import core;

/// 实体也是一个组件
export template <typename T>
    requires std::unsigned_integral<T>
class BasicEntity : public Component
{
  public:
    using ValueType = T;
    using IdType = T;
    using VersionType = T;

  public:
    // 偏移id，是因为计算id更加简单
#pragma region IdOffset
    template <typename>
    struct IdOffset;

    template <>
    struct IdOffset<u32>
    {
        static constexpr usize value = 12;
    };

    template <>
    struct IdOffset<u64>
    {
        static constexpr usize value = 24;
    };

  public:
    static constexpr usize id_offset = IdOffset<T>::value;
#pragma endregion // IdOffset

  public:
    BasicEntity()
        : entity{}
    {
    }

    explicit BasicEntity( T id, T version )
        : entity( id << id_offset | version )
    {
    }

  public:
    IdType GetId() const
    {
        return entity >> id_offset;
    }

    VersionType GetVersion() const
    {
        return entity & ( ( 1u << id_offset ) - 1u );
    }

    void UpgradeVersion()
    {
        entity = ( entity & ~( ( 1u << id_offset ) - 1u ) ) | ( GetVersion() + 1u );
    }

    bool IsValid() const
    {
        return GetId() != 0;
    }

  public:
    T entity;
};

export template <typename T>
    requires std::unsigned_integral<T>
bool operator==( const BasicEntity<T>& lhs, const BasicEntity<T>& rhs )
{
    return lhs.entity == rhs.entity;
}

/// std::unordered_map 的 Key 需要重载hash函数
export template <typename T>
    requires std::unsigned_integral<T>
struct ECS_API std::hash<BasicEntity<T>>
{
    size_t operator()( const BasicEntity<T>& entity ) const
    {
        return std::hash<T>()( entity.entity );
    }
};

export template <typename T>
struct IsEntity : std::false_type
{
};

export template <typename U>
struct IsEntity<BasicEntity<U>> : std::true_type
{
};

export template <typename T>
inline constexpr bool IsEntityValue = IsEntity<T>::value;

export template <typename T>
concept IsEntityConcept = IsEntityValue<T>;

export using Entity = BasicEntity<u32>;

export ECS_API inline const Entity InvalidEntity = Entity( 0, 0 );