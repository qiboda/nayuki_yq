module;

#include "module_export.h"

export module ecs.schedule:system_set_node_config;

import :base;
import :config_node;
import :schedule_graph;

import ecs.schedule.graph;
import ecs.systems.concepts;
import std;

export class ECS_API ScheduleSystemSetNodeConfig
{
  protected:
    ScheduleSystemSetNodeConfig()
        : mChainFunction( nullptr )
    {
    }

  public:
    template <IsSystemSetConcept... T>
    static ScheduleSystemSetNodeConfig Create();

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& After();

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& Before();

    template <IsSystemConcept Func>
    ScheduleSystemSetNodeConfig& After( Func func );

    template <IsSystemConcept Func>
    ScheduleSystemSetNodeConfig& Before( Func func );

    ScheduleSystemSetNodeConfig& Chain();

    template <IsSystemSetConcept T>
    ScheduleSystemSetNodeConfig& InSet();

    ScheduleSystemSetNodeConfig Build()
    {
        return *this;
    }

    void Apply( std::shared_ptr<class ScheduleBase> Schedule )
    {
        ScheduleNodeIdChainType Chain;
        for ( auto&& curFunc : mCurNodesFunction )
        {
            auto scheduleNodeId = curFunc( Schedule );
            Chain.push_back( scheduleNodeId );

            for ( auto&& func : mOperateFunctions )
            {
                func( Schedule, scheduleNodeId );
            }

            mOperateFunctions.clear();
        }

        if ( mChainFunction )
        {
            mChainFunction( Schedule, std::move( Chain ) );
        }
    }

  private:
    std::vector<std::function<ScheduleNodeId( std::shared_ptr<class ScheduleBase> )>> mCurNodesFunction;
    std::vector<std::function<void( std::shared_ptr<class ScheduleBase>, ScheduleNodeId curNodeId )>> mOperateFunctions;
    std::function<void( std::shared_ptr<class ScheduleBase>, ScheduleNodeIdChainType&& )> mChainFunction = nullptr;
};

template <IsSystemSetConcept... T>
ScheduleSystemSetNodeConfig ScheduleSystemSetNodeConfig::Create()
{
    auto config = ScheduleSystemSetNodeConfig();
    ( config.mCurNodesFunction.push_back(
          []( std::shared_ptr<class ScheduleBase> schedule )
          {
              return schedule->GetScheduleGraph()->AddSystemSetInConfig<T>();
          } ),
      ... );
    return config;
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::After()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->AfterSystemSetInConfig<T>( curNodeId );
        } );

    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::Before()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->BeforeSystemSetInConfig<T>( curNodeId );
        } );

    return *this;
}

template <IsSystemConcept Func>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::After( Func func )
{
    mOperateFunctions.push_back(
        [func]( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->AfterSystemInConfig( func, curNodeId );
        } );

    return *this;
}

template <IsSystemConcept Func>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::Before( Func func )
{
    mOperateFunctions.push_back(
        [func]( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->BeforeSystemInConfig( curNodeId, func );
        } );

    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::InSet()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<class ScheduleBase> schedule, ScheduleNodeId curNodeId )
        {
            schedule->GetScheduleGraph()->InSetInConfig<T>( curNodeId );
        } );

    return *this;
}
