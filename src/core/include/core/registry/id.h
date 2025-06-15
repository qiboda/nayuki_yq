#pragma once

#include <core/minimal.h>

struct Id;

template <typename T>
concept IsId = IsStrictDerivedConcept<Id, T>;

struct ECS_API Id
{
    template <IsId TId, typename TBaseType>
    friend class IdRegistry;

    template <IsId TId>
    friend class IdGenerator;

    constexpr Id()
        : mId( std::numeric_limits<u32>::max() )
    {
    }

    constexpr friend bool operator==( const Id& lhs, const Id& rhs )
    {
        return lhs.mId == rhs.mId;
    }

    constexpr u32 Index() const
    {
        return mId;
    }

  protected:
    constexpr void SetId( u32 id )
    {
        mId = id;
    }

  protected:
    u32 mId = std::numeric_limits<u32>::max();
};

template <IsId TId, typename TBaseType>
class IdRegistry
{
  public:
    template <typename TType>
        requires IsStrictDerivedConcept<TBaseType, TType>
    static TId Get()
    {
        static const TId id = Next();
        return id;
    }

  private:
    static constexpr TId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static inline TId sId = TId();
};

template <IsId TId>
class IdGenerator
{
  public:
    static constexpr TId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static inline TId sId = TId();
};