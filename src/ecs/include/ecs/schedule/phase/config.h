#pragma once

#include "ecs/schedule/graph/graph.h"
#include "ecs/schedule/phase/phase.h"
#include <core/container/small_vector.h>
#include <range/v3/all.hpp>
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API PhaseInfo
{
    PhaseId mId;
    std::string mName;
};

struct ECS_API PhaseEdge
{
    // edge的起点
    PhaseId mOut;
    // edge的终点
    PhaseId mIn;

    friend auto operator<=>( const PhaseEdge& lhs, const PhaseEdge& rhs ) = default;
};

struct ECS_API PhaseConfigure
{
    friend struct PhaseConfigureSet;

    template <IsSchedulePhase... T>
    PhaseConfigure& Init()
    {
        mPhases = { PhaseIdRegistry::Get<T>()... };
        ( mPhaseInfos.emplace( PhaseIdRegistry::Get<T>(),
                               PhaseInfo{ .mId = PhaseIdRegistry::Get<T>(), .mName = std::format( "{}", T{} ) } ),
          ... );
        return *this;
    }

    /**
     * @brief T -> self, 先执行T，后执行self
     */
    template <IsSchedulePhase T>
    PhaseConfigure& After()
    {
        PhaseId id = PhaseIdRegistry::Get<T>();
        mOtherPhases.insert( id );
        mPhaseInfos.emplace( PhaseIdRegistry::Get<T>(),
                             PhaseInfo{ .mId = PhaseIdRegistry::Get<T>(), .mName = std::format( "{}", T{} ) } );
        for ( auto&& phase : mPhases )
        {
            mEdge.insert( PhaseEdge{
                .mOut = id,
                .mIn = phase,
            } );
        }
        return *this;
    }

    /**
     * @brief self -> T , 先执行 Self，后执行 T
     */
    template <IsSchedulePhase T>
    PhaseConfigure& Before()
    {
        PhaseId id = PhaseIdRegistry::Get<T>();
        mOtherPhases.insert( id );
        mPhaseInfos.emplace( PhaseIdRegistry::Get<T>(),
                             PhaseInfo{ .mId = PhaseIdRegistry::Get<T>(), .mName = std::format( "{}", T{} ) } );
        for ( auto&& phase : mPhases )
        {
            mEdge.insert( PhaseEdge{
                .mOut = phase,
                .mIn = id,
            } );
        }
        return *this;
    }

    PhaseConfigure& Chain()
    {
        for ( auto phaseWindow : mPhases | ranges::views::sliding( 2 ) )
        {
            auto begin = phaseWindow.begin();
            mEdge.insert( PhaseEdge{
                .mOut = *begin,
                .mIn = *++begin,
            } );
        }
        return *this;
    }

  protected:
    std::set<PhaseId> mPhases;
    std::set<PhaseId> mOtherPhases;

    std::set<PhaseEdge> mEdge;

    std::map<PhaseId, PhaseInfo> mPhaseInfos;
};

template <IsSchedulePhase... T>
struct PhaseConfigureBuilder
{
    PhaseConfigureBuilder()
        : mConfig()
    {
        mConfig.Init<T...>();
    }

    /**
     * @brief O -> self, 先执行 O，后执行self
     */
    template <IsSchedulePhase O>
    PhaseConfigureBuilder& After()
    {
        mConfig.After<O>();
        return *this;
    }

    /** @brief self -> O , 先执行 Self，后执行 O
     *
     */
    template <IsSchedulePhase O>
    PhaseConfigureBuilder& Before()
    {
        mConfig.Before<O>();
        return *this;
    }

    PhaseConfigureBuilder& Chain()
    {
        mConfig.Chain();
        return *this;
    }

    PhaseConfigure End()
    {
        return std::move( mConfig );
    }

  protected:
    PhaseConfigure mConfig;
};

struct ECS_API PhaseConfigureSet
{
  public:
    void Configure( PhaseConfigure&& configure )
    {
        mPhaseInfos.insert( configure.mPhaseInfos.begin(), configure.mPhaseInfos.end() );
        mEdge.insert( configure.mEdge.begin(), configure.mEdge.end() );
    }

    Graph<PhaseId, PhaseInfo> BuildGraph()
    {
        Graph<PhaseId, PhaseInfo> graph;
        for ( auto& edge : mEdge )
        {
            graph.AddEdge( edge.mOut, edge.mIn );
        }
        for ( auto info : mPhaseInfos )
        {
            graph.AddNode( info.first, info.second );
        }
        return graph;
    }

  protected:
    std::set<PhaseEdge> mEdge;
    std::map<PhaseId, PhaseInfo> mPhaseInfos;
};
