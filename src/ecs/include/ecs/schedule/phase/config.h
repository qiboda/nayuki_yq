#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/container/small_vector.h>
#include <range/v3/all.hpp>
#include <core/minimal.h>
#include <ecs/minimal.h>

struct PhaseEdge
{
    // edge的起点
    PhaseId mOut;
    // edge的终点
    PhaseId mIn;

    friend auto operator<=>( const PhaseEdge& lhs, const PhaseEdge& rhs ) = default;
};

struct PhaseConfigure
{
    template <IsSchedulePhase... T>
    PhaseConfigure& init()
    {
        mPhases = { PhaseIdRegistry::Get<T>()... };
        return *this;
    }

    template <IsSchedulePhase T>
    PhaseConfigure& Before()
    {
        PhaseId id = PhaseIdRegistry::Get<T>();
        mOtherPhases.insert( id );
        for ( auto&& phase : mPhases )
        {
            Edge.insert( PhaseEdge{
                .mOut = id,
                .mIn = phase,
            } );
        }
        return *this;
    }

    template <IsSchedulePhase T>
    PhaseConfigure& After()
    {
        PhaseId id = PhaseIdRegistry::Get<T>();
        mOtherPhases.insert( id );
        for ( auto&& phase : mPhases )
        {
            Edge.insert( PhaseEdge{
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
            auto second = ++begin;
            Edge.insert( PhaseEdge{
                .mOut = *begin,
                .mIn = *second,
            } );
        }
        return *this;
    }

  protected:
    std::set<PhaseId> mPhases;
    std::set<PhaseId> mOtherPhases;

    std::set<PhaseEdge> Edge;
};

template <IsSchedulePhase... T>
struct PhaseConfigureBuilder
{
    PhaseConfigureBuilder()
        : mConfig()
    {
        mConfig.init<T...>();
    }

    template <IsSchedulePhase O>
    PhaseConfigureBuilder& Before()
    {
        mConfig.Before<O>();
        return *this;
    }

    template <IsSchedulePhase O>
    PhaseConfigureBuilder& After()
    {
        mConfig.After<O>();
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

struct PhaseConfigureSet
{
  public:
    void configure( PhaseConfigure&& configure )
    {
        mConfigures.push_back( configure );
    }

  protected:
    std::vector<PhaseConfigure> mConfigures;
};
