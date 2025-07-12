module;

#include "module_export.h"
#include <core/macro/macro.h>
#include <fmt/format.h>

export module ecs.schedule.graph;

import core;
import std;

export template <typename TNodeId>
class Topology
{
  public:
    void AddLayer( std::set<TNodeId>&& layer )
    {
        mPhaseSetLayers.emplace_back( std::move( layer ) );
    }

    const std::set<TNodeId>& GetLayer( usize index ) const
    {
        return mPhaseSetLayers[index];
    }

    usize LayerNum() const
    {
        return mPhaseSetLayers.size();
    }

    void Clear()
    {
        mPhaseSetLayers.clear();
    }

  protected:
    std::vector<std::set<TNodeId>> mPhaseSetLayers;
};

export template <typename TNodeId, typename TNode>
class Graph
{
    template <typename TTNodeId, typename TTNode>
    friend class TarjanGraph;

  public:
    Graph()
    {
    }

  public:
    void AddNode( TNodeId id, TNode& node )
    {
        mNodes.emplace( id, node );
    }

    void AddNode( TNodeId id, TNode&& node )
    {
        mNodes.emplace( id, node );
    }

    const TNode& GetNode( TNodeId id ) const
    {
        return mNodes.find( id )->second;
    }

    void AddEdge( TNodeId out, TNodeId in )
    {
        mEdges.insert( { out, in } );
    }

    // Transitive Closure（传递闭包），这样才能正确根据入度做拓扑排序。
    void TransitiveClosure()
    {
    }

    // Transitive Reduction（传递规约）
    void TransitiveReduction()
    {
    }

    /**
     * @brief 拓扑排序
     * Kahn 算法
     */
    void TopSort()
    {
        std::unordered_map<TNodeId, i32> nodeInNums;
        // 统计有入度的节点
        for ( auto&& [idOut, idIn] : mEdges )
        {
            auto result = nodeInNums.insert( { idIn, 1 } );
            if ( result.second == false )
            {
                result.first->second++;
            }
        }

        // 统计没有入度的节点
        for ( auto&& [id, node] : mNodes )
        {
            if ( nodeInNums.contains( id ) == false )
            {
                nodeInNums.insert( { id, 0 } );
            }
        }

        mTopology.Clear();
        while ( nodeInNums.size() > 0 )
        {
            std::set<TNodeId> layer;
            std::map<TNodeId, i32> nodeDecreaseCountMap;
            std::erase_if( nodeInNums,
                           [&layer, this, &nodeDecreaseCountMap]( auto& pair )
                           {
                               if ( pair.second <= 0 )
                               {
                                   layer.insert( pair.first );

                                   auto range = mEdges.equal_range( pair.first );
                                   for ( auto it = range.first; it != range.second; ++it )
                                   {
                                       nodeDecreaseCountMap[it->second] += 1;
                                   }
                                   return true;
                               }
                               return false;
                           } );

            for ( auto&& [nodeId, decreaseCount] : std::move( nodeDecreaseCountMap ) )
            {
                auto it = nodeInNums.find( nodeId );
                if ( it != nodeInNums.end() )
                {
                    it->second -= decreaseCount;
                }
            }

            NY_ASSERT_MSG( layer.size() > 0, "Topology has a circle at least." );

            mTopology.AddLayer( std::move( layer ) );
        }
    }

    const Topology<TNodeId>& GetTopology() const
    {
        return mTopology;
    }

  protected:
    // 这里使用map而不是unordered map, 是为了TarjanGraph的计算结果保持有序。
    std::map<TNodeId, TNode> mNodes;
    std::unordered_multimap<TNodeId, TNodeId> mEdges;

    Topology<TNodeId> mTopology;
};
