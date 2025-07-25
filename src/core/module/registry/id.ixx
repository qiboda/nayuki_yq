module;

#include <module_export.h>

export module core.registry;

import core;
import core.misc;
import core.concepts.derived;
import std;

export template <typename T>
concept IsId = IsStrictDerivedConcept<struct CORE_API Id, T>;

export struct CORE_API Id
{
    template <IsId TId, typename TBaseType>
    friend class IdRegistry;

    template <IsId TId>
    friend class IdGenerator;

  public:
    Id()
    {
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

export auto operator<=>( const Id& lhs, const Id& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

export template <IsId TId, typename TBaseType>
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
    static TId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static inline TId sId = TId();
};

export template <IsId TId>
class IdGenerator
{
  public:
    static TId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static inline TId sId = TId();
};