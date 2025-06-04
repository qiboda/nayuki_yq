#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>
#include <ecs/schedule/graph/graph.h>

/**
 * @brief SCC(强连通分量) 环检测
 */
template <typename TNodeId, typename TNode>
class ECS_API TarjanGraph
{
  public:
    TarjanGraph( Graph<TNodeId, TNode>& graph )
        : graph( graph )
    {
    }

    void RunTarjan()
    {
        for ( const auto& [nodeId, node] : graph.mNodes )
        {
            if ( mIndices.find( nodeId ) == mIndices.end() )
            {
                Dfs( nodeId );
            }
        }
    }

    const std::vector<std::vector<TNodeId>>& GetSCCs() const
    {
        return mSCCs;
    }

  protected:
    void Dfs( const TNodeId& v )
    {
        mIndices[v] = mLowlink[v] = mCurrentIndex++;
        mStack.push( v );
        mOnStack[v] = true;

        // 遍历邻居
        auto range = graph.mEdges.equal_range( v );
        for ( auto it = range.first; it != range.second; ++it )
        {
            const TNodeId& w = it->second;

            if ( mIndices.find( w ) == mIndices.end() )
            {
                Dfs( w );
                mLowlink[v] = std::min( mLowlink[v], mLowlink[w] );
            }
            // 如果是回边，使用后续的节点(本质是前面的节点)的index更新lowlink
            else if ( mOnStack[w] )
            {
                mLowlink[v] = std::min( mLowlink[v], mIndices[w] );
            }
        }

        // 如果 v 是 SCC 的根
        if ( mLowlink[v] == mIndices[v] )
        {
            std::vector<TNodeId> scc;
            TNodeId w;
            do
            {
                w = mStack.top();
                mStack.pop();
                mOnStack[w] = false;
                scc.push_back( w );
            } while ( w != v );
            mSCCs.push_back( std::move( scc ) );
        }
    }

  protected:
    std::unordered_map<TNodeId, i32> mIndices;  // 时间戳
    std::unordered_map<TNodeId, i32> mLowlink;  // 能到的最小 index
    std::unordered_map<TNodeId, bool> mOnStack; // 是否在当前 DFS 栈上
    std::stack<TNodeId> mStack;                 // 当前 DFS 路径上的节点
    i32 mCurrentIndex = 0;

    std::vector<std::vector<TNodeId>> mSCCs; // 最终结果

    Graph<TNodeId, TNode>& graph;
};