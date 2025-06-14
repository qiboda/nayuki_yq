#pragma once

#include "ecs/schedule/config/node.h"
#include "ecs/schedule/config/system_set.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <range/v3/view/sliding.hpp>

/**
 * @brief 假设了 Add 函数接口总是最先被调用。
 */
class ECS_API ScheduleSystemNodeConfig
{
  public:
    /**
     * @brief Construct a new Schedule System Node Config object
     *
     * @param func 作为mCurNodes
     */
    template <IsSystemConcept Func>
    ScheduleSystemNodeConfig( Func func );

    // template <IsSystemConcept... Func>
    // ScheduleSystemNodeConfig( Func... func );

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

    // void Chain()
    // {
    // for ( const auto& sliding : mCurNodes | ranges::views::sliding( 2 ) )
    // {
    //     auto begin = sliding.begin();
    //     mEdgeMap.insert( { *begin, *begin++ } );
    // }
    // }

    template <IsSystemSetConcept T>
    ScheduleSystemNodeConfig& InSet();

    void Apply( std::shared_ptr<class ScheduleBase> Schedule )
    {
        ScheduleNodeId nodeId = mCurNodesFunctions( Schedule );
        for ( auto&& func : mOperateFunctions )
        {
            func( Schedule, nodeId );
        }
        mOperateFunctions.clear();
    }

  protected:
    // 第一个也是唯一一个是获得主 system nodes.
    std::function<ScheduleNodeId( std::shared_ptr<class ScheduleBase> Schedule )> mCurNodesFunctions;
    std::vector<std::function<void( std::shared_ptr<class ScheduleBase> Schedule, ScheduleNodeId )>> mOperateFunctions;
};

#include "ecs/schedule/schedule_base.h"
#include "ecs/schedule/schedule_graph.h"

template <IsSystemConcept Func>
ScheduleSystemNodeConfig::ScheduleSystemNodeConfig( Func func )
{
    mOperateFunctions.push_back(
        [func = std::move( func )]( std::shared_ptr<ScheduleBase> Schedule ) -> ScheduleNodeId
        {
            return Schedule->GetScheduleGraph()->AddSystemInConfig( func );
        } );
}

template <IsSystemConcept Func>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::After( Func func )
{
    mOperateFunctions.push_back(
        [func = std::move( func )]( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->AfterSystemInConfig( std::move( func ), curNodeId );
        } );
    return *this;
}

template <IsSystemConcept Func>
ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::Before( Func func )
{
    mOperateFunctions.push_back(
        [func = std::move( func )]( std::shared_ptr<ScheduleBase> Schedule, ScheduleNodeId curNodeId )
        {
            Schedule->GetScheduleGraph()->BeforeSystemInConfig( curNodeId, std::move( func ) );
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