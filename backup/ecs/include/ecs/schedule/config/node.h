#pragma once

import core.container;
import core.misc;
#include "ecs/schedule/config/system_set.h"
#include "ecs/systems/system.h"

class ECS_API ScheduleNode
{
};

struct ScheduleNodeId : public Id
{
    friend bool operator==( const ScheduleNodeId& lhs, const ScheduleNodeId& rhs ) = default;

    friend bool operator<( const ScheduleNodeId& lhs, const ScheduleNodeId& rhs )
    {
        return lhs.Index() < rhs.Index();
    }
};

template <>
struct std::hash<ScheduleNodeId>
{
    usize operator()( const ScheduleNodeId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

struct ECS_API ScheduleNodeIdGenerator : public IdGenerator<ScheduleNodeId>
{
};

class ECS_API ScheduleSystemNode : public ScheduleNode, public NonCopyable
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

class ECS_API ScheduleSystemSetNode : public ScheduleNode, public NonCopyable
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

class ECS_API ScheduleNodeVariant : public NonCopyable
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

using ScheduleNodeIdChainType = SmallVector<ScheduleNodeId, 8>;
