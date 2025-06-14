#pragma once

#include "ecs/schedule/config/node.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <range/v3/view/sliding.hpp>

class ECS_API ScheduleSystemSetNodeConfig
{
  public:
    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig();

    // ScheduleSystemSetNodeConfig( std::initializer_list<ScheduleSystemSetNode>&& nodes )
    // {
    // }

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& After();

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& Before();

    // void Chain()
    // {
    //     for ( auto sliding : mCurNodes | ranges::views::sliding( 2 ) )
    //     {
    //         auto begin = sliding.begin();
    //         mEdgeMap.insert( { *begin, *begin++ } );
    //     }
    // }

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& InSet();

    void Apply( std::shared_ptr<class ScheduleBase> Schedule )
    {
        auto scheduleNodeId = mCurNodesFunction( Schedule );
        for ( auto&& func : mOperateFunctions )
        {
            func( Schedule, scheduleNodeId );
        }
        mOperateFunctions.clear();
    }

  private:
    std::function<ScheduleNodeId( std::shared_ptr<class ScheduleBase> )> mCurNodesFunction;
    std::vector<std::function<void( std::shared_ptr<class ScheduleBase>, ScheduleNodeId curNodeId )>> mOperateFunctions;
};

#include "ecs/schedule/schedule_base.h"
#include "ecs/schedule/schedule_graph.h"

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig::ScheduleSystemSetNodeConfig()
{
    mCurNodesFunction = []( std::shared_ptr<class ScheduleBase> schedule )
    {
        schedule->GetScheduleGraph()->AddSystemSetInConfig<T>();
    };
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::After()
{
    mOperateFunctions = []( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
    {
        schedule->GetScheduleGraph()->AfterSystemSetInConfig<T>( curNodeId );
    };

    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::Before()
{
    mOperateFunctions = []( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
    {
        schedule->GetScheduleGraph()->BeforeSystemSetInConfig<T>( curNodeId );
    };

    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::InSet()
{
    mOperateFunctions.push_back(
        [this]( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->InSetInConfig<T>( curNodeId );
        } );

    return *this;
}