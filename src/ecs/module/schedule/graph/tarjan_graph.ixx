module;

#include "module_export.h"

export module ecs.tarjan_graph;

import ecs.graph;
import std;
import core;

/**
 * @brief SCC(强连通分量) 环检测
 * 算法简述：
 *   构成环的特征是下一个节点是之前访问过的节点。根据这一特征，将构成环的节点标记为一组输出。
 *   此外，应注意，如果下一个节点已经被标记为一组输出了，就意味着这个节点是终点了。
 *   该节点所在的路径是一个分叉，并不是回路。
 *
 * 该算法并不保证SCC内部的顺序和SCC之间的顺序保持固定。
 * 目前的实现也没有做这个保证。看情况之后自行排序。
 *
 * TODO：优化数据结构，降低内存占用和 map查询的性能损耗
 */
export template <typename TNodeId, typename TNode>
class TarjanGraph
{
  public:
    TarjanGraph( Graph<TNodeId, TNode>& graph )
        : mGraph( graph )
    {
    }

    void RunTarjan()
    {
        for ( const auto& [nodeId, node] : mGraph.mNodes )
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
        auto range = mGraph.mEdges.equal_range( v );
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

    Graph<TNodeId, TNode>& mGraph;
};