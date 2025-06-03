#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename TNodeId>
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

template <typename TNodeId, typename TNode>
class Graph
{
  public:
    Graph()
    {
    }

  public:
    void AddNode( TNodeId id, TNode& node )
    {
        mNodes.emplace( id, node );
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
        std::unordered_map<TNodeId, u32> nodeInNums;
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
            std::erase_if( nodeInNums,
                           [&layer]( auto& pair )
                           {
                               if ( pair.second == 0 )
                               {
                                   layer.insert( pair.first );
                                   return true;
                               }
                               else
                               {
                                   pair.second -= 1;
                                   return false;
                               }
                           } );

            NY_ASSERT_MSG( layer.size() > 0, "Topology has a circle at least." )

            mTopology.AddLayer( std::move( layer ) );
        }
    }

    const Topology<TNodeId>& GetTopology() const
    {
        return mTopology;
    }

  protected:
    std::unordered_map<TNodeId, TNode> mNodes;
    std::unordered_multimap<TNodeId, TNodeId> mEdges;

    Topology<TNodeId> mTopology;
};
