module;

#include "module_export.h"
#include <memory>

export module ecs:schedule_system_node_config;

import :schedule_config_node;
import :schedule_system_set;
import :schedule_graph;
import :schedule_base;
import ecs.graph;
import :system_concepts;

// import std;

/**
 * @brief 假设了 Add 函数接口总是最先被调用。
 */
export class ECS_API ScheduleSystemNodeConfig
{
  protected:
    ScheduleSystemNodeConfig()
    {
    }

  public:
    template <IsSystemConcept... Func>
    static ScheduleSystemNodeConfig Create( Func... func );

    /**
     * @brief func -> mCurNodes
     */
    template <IsSystemConcept Func>
    ScheduleSystemNodeConfig& After( Func func );

    /**
     * @brief systemSet -> mCurNodes
     */
    template <IsSystemSetConcept T>
    ScheduleSystemNodeConfig& After();

    /**
     * @brief mCurNodes -> func
     */
    template <IsSystemConcept Func>
    ScheduleSystemNodeConfig& Before( Func func );

    /**
     * @brief mCurNodes -> systemSet
     */
    template <IsSystemSetConcept T>
    ScheduleSystemNodeConfig& Before();

    ScheduleSystemNodeConfig& Chain();

    template <IsSystemSetConcept T>
    ScheduleSystemNodeConfig& InSet();

    ScheduleSystemNodeConfig Build()
    {
        return *this;
    }

    void Apply( std::shared_ptr<class ScheduleBase> Schedule )
    {
        ScheduleNodeIdChainType Chain;
        for ( auto&& curFunc : mCurNodesFunctions )
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

  protected:
    // 第一个也是唯一一个是获得主 system nodes.
    std::vector<std::function<ScheduleNodeId( std::shared_ptr<class ScheduleBase> Schedule )>> mCurNodesFunctions;
    std::vector<std::function<void( std::shared_ptr<class ScheduleBase> Schedule, ScheduleNodeId )>> mOperateFunctions;
    std::function<void( std::shared_ptr<class ScheduleBase> Schedule, ScheduleNodeIdChainType&& )> mChainFunction =
        nullptr;
};

template <IsSystemConcept... Func>
ScheduleSystemNodeConfig ScheduleSystemNodeConfig::Create( Func... func )
{
    ScheduleSystemNodeConfig config;
    ( config.mCurNodesFunctions.push_back(
          [func]( std::shared_ptr<ScheduleBase> Schedule ) -> ScheduleNodeId
          {
              return Schedule->GetScheduleGraph()->AddSystemInConfig( func );
          } ),
      ... );
    return config;
}

template <IsSystemConcept Func>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::After( Func func )
{
    mOperateFunctions.push_back(
        [func]( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->AfterSystemInConfig( func, curNodeId );
        } );
    return *this;
}

template <IsSystemConcept Func>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::Before( Func func )
{
    mOperateFunctions.push_back(
        [func]( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->BeforeSystemInConfig( curNodeId, func );
        } );
    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::After()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->AfterSystemSetInConfig<T>( curNodeId );
        } );
    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::Before()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->BeforeSystemSetInConfig<T>( curNodeId );
        } );
    return *this;
}

template <IsSystemSetConcept T>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::InSet()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->InSetInConfig<T>( curNodeId );
        } );
    return *this;
}