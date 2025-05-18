#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename T>
    requires std::integral<T>
class BasicEntity
{
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
    BasicEntity() : entity{}
    {
    }

    explicit BasicEntity( T entity ) : entity( entity )
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