#pragma once

#include "ecs/schedule/config/nodes/system_node.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API SystemNodeGraph
{
  public:
    SystemNodeGraph();

  public:
    void AddNode( ScheduleSystemNode&& node )
    {
        // mNodes.emplace_back( std::move( node ) );
        UNUSED_VAR( node );
    }

    void AddNodeSet( ScheduleSystemNodeConfig&& nodeSet )
    {
        UNUSED_VAR( nodeSet );
        // for ( const auto& node : nodeSet.mNodes )
        // {
        //     mNodes.push_back( node );
        // }
    }

  protected:
    // std::unordered_map<ScheduleNodeId, ScheduleSystemNode> AllNodes;
    // std::unordered_map<ScheduleNodeId, ScheduleNodeId> AllEdges;
};