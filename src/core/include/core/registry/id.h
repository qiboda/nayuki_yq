#pragma once

#include <core/minimal.h>

struct Id;

template <typename T>
concept IsId = std::is_base_of_v<Id, T>;

struct ECS_API Id
{
    template <IsId TId, typename TBaseType>
    friend class IdRegistry;

    Id()
        : mId( std::numeric_limits<u32>::max() )
    {
    }

    friend bool operator==( const Id& lhs, const Id& rhs )
    {
        return lhs.mId == rhs.mId;
    }

    u32 Index() const
    {
        return mId;
    }

  protected:
    void SetId( u32 id )
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
        requires std::is_base_of_v<TBaseType, TType>
    static TId Get()
    {
        static const TId id = Next();
        return id;
    }

  private:
    static TId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static inline TId sId = TId();
};