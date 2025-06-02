#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

/// system函数，到 Node
/// System Set 到node。
/// Graph 也是一个 Node
///

struct GraphNodeId
{
    usize mId = 0;

    GraphNodeId() = default;
    GraphNodeId( const GraphNodeId& ) = default;

    friend auto operator<=>( const GraphNodeId& lhs, const GraphNodeId& rhs ) = default;
};

template <>
struct std::hash<GraphNodeId>
{
    usize operator()( const GraphNodeId& id ) const
    {
        return std::hash<usize>()( id.mId );
    }
};

class GraphNode
{
    GraphNodeId mId;
    std::string mName;
};

class ECS_API Graph
{
  public:
    Graph();

  public:
    void AddEdge()
    {
    }

  protected:
    std::unordered_map<GraphNodeId, GraphNode> mNodes;
    std::unordered_map<GraphNodeId, GraphNodeId> mEdges;
};
