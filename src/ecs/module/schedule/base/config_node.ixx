module;

#include "module_export.h"

export module ecs.schedule:config_node;

import :system_set;

import std;
import core.registry;
import core.container.small_vector;
import core.misc.non_copyable;
import core.type;
import ecs.systems.id;

export class ECS_API ScheduleNode
{
};

export struct ECS_API ScheduleNodeId : public Id
{
};

export ECS_API inline auto operator<=>( const ScheduleNodeId& lhs, const ScheduleNodeId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

export ECS_API inline bool operator==( const ScheduleNodeId& lhs, const ScheduleNodeId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export template <>
struct std::hash<ScheduleNodeId>
{
    usize operator()( const ScheduleNodeId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

export struct ECS_API ScheduleNodeIdGenerator : public IdGenerator<ScheduleNodeId>
{
};

export class ECS_API ScheduleSystemNode : public ScheduleNode, public NonCopyable
{
  public:
    ScheduleSystemNode( SystemId systemId )
        : mSystemId( systemId )
    {
    }

    ScheduleSystemNode( ScheduleSystemNode&& node )
        : mSystemId( std::move( node.mSystemId ) )
    {
    }

    SystemId GetSystemId() const
    {
        return mSystemId;
    }

  private:
    SystemId mSystemId;
};

export class ECS_API ScheduleSystemSetNode : public ScheduleNode, public NonCopyable
{
  public:
    ScheduleSystemSetNode( SystemSetId setId )
        : mSetId( setId )
    {
    }

    template <IsSystemSetConcept T>
    ScheduleSystemSetNode()
        : mSetId( SystemSetIdRegistry::Get<T>() )
    {
    }

    ScheduleSystemSetNode( ScheduleSystemSetNode&& node )
        : mSetId( std::move( node.mSetId ) )
    {
    }

    SystemSetId GetSetId() const
    {
        return mSetId;
    }

  private:
    SystemSetId mSetId;
};

export class ECS_API ScheduleNodeVariant : public NonCopyable
{
  public:
    using NodeType = std::variant<ScheduleSystemNode, ScheduleSystemSetNode>;

    ScheduleNodeVariant( ScheduleSystemNode&& node )
        : mNode( std::in_place_type<ScheduleSystemNode>, std::move( node ) )
    {
    }

    ScheduleNodeVariant( ScheduleSystemSetNode&& node )
        : mNode( std::in_place_type<ScheduleSystemSetNode>, std::move( node ) )
    {
    }

    template <typename T>
    const T& Get() const
    {
        return std::get<T>( mNode );
    }

    template <typename T>
    bool Is() const
    {
        return std::holds_alternative<T>( mNode );
    }

  private:
    NodeType mNode;
};

export using ScheduleNodeIdChainType = SmallVector<ScheduleNodeId, 8>;
