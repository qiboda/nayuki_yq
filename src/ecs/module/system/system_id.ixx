module;

#include "module_export.h"

export module ecs:system_id;

import core;
import std;
import :system_concepts;
import core.registry;

export struct ECS_API SystemId : public Id
{
    friend class SystemIdRegistry;

  public:
    static const SystemId Invalid;
};

export ECS_API inline bool operator==( const SystemId& lhs, const SystemId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export ECS_API inline auto operator<=>( const SystemId& lhs, const SystemId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

export template <>
struct ECS_API std::hash<SystemId>
{
    size_t operator()( SystemId systemId ) const noexcept
    {
        return std::hash<usize>()( systemId.Index() );
    }
};

export class ECS_API SystemIdRegistry
{
  public:
    template <IsSystemConcept Func>
    static SystemId Get( Func func )
    {
        auto it = mFuncToIdMap.find( reinterpret_cast<void*>( func ) );
        if ( it != mFuncToIdMap.end() )
        {
            return it->second;
        }

        return SystemId::Invalid;
    }

    template <IsSystemConcept Func>
    static SystemId Add( Func func )
    {
        auto systemId = Next();
        mFuncToIdMap.emplace( reinterpret_cast<void*>( func ), systemId );
        return systemId;
    }

    template <IsSystemConcept Func>
    static SystemId GetOrAdd( Func func )
    {
        auto systemId = Get( func );
        if ( systemId != SystemId::Invalid )
        {
            return systemId;
        }

        return Add( func );
    }

  protected:
    static SystemId Next()
    {
        auto index = sId.Index() + 1;
        sId.SetId( index );
        return sId;
    }

  protected:
    static SystemId sId;

    static std::unordered_map<void*, SystemId> mFuncToIdMap;
};