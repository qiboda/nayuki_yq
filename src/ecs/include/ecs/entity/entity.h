#pragma once

#include "ecs/components/component.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <type_traits>

/// 实体也是一个组件
template <typename T>
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

template <typename T>
    requires std::unsigned_integral<T>
bool operator==( const BasicEntity<T>& lhs, const BasicEntity<T>& rhs )
{
    return lhs.entity == rhs.entity;
}

/// std::unordered_map 的 Key 需要重载hash函数
template <typename T>
    requires std::unsigned_integral<T>
struct std::hash<BasicEntity<T>>
{
    size_t operator()( const BasicEntity<T>& entity ) const
    {
        return std::hash<T>()( entity.entity );
    }
};

template <typename T>
struct IsEntity : std::false_type
{
};

template <typename U>
struct IsEntity<BasicEntity<U>> : std::true_type
{
};

template <typename T>
inline constexpr bool IsEntityValue = IsEntity<T>::value;

template <typename T>
concept IsEntityConcept = IsEntityValue<T>;

using Entity = BasicEntity<u32>;

static const Entity InvalidEntity = Entity( 0, 0 );