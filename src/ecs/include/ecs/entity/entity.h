#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename T>
    requires std::integral<T>
class BasicEntity
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
        static constexpr size_t value = 12;
    };

    template <>
    struct IdOffset<u64>
    {
        static constexpr size_t value = 24;
    };

  public:
    static constexpr size_t id_offset = IdOffset<T>::value;
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
    T GetId() const
    {
        return entity >> id_offset;
    }

    T GetVersion() const
    {
        return entity & ( ( 1 << id_offset ) - 1 );
    }

    void UpgradeVersion()
    {
        entity = ( entity & ~( ( 1 << id_offset ) - 1 ) ) | ( GetVersion() + 1 );
    }

    bool IsValid() const
    {
        return GetId() != 0;
    }

  public:
    T entity;
};

template <typename T>
struct IsEntity : std::false_type
{
};

template <typename T>
struct IsEntity<BasicEntity<T>> : std::true_type
{
};

template <typename T>
concept IsEntityConcept = IsEntity<T>::value;

using Entity = BasicEntity<u32>;

static const Entity InvalidEntity = Entity( 0, 0 );