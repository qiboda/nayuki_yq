#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/container/small_vector.h>
#include <range/v3/all.hpp>
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API PhaseConfigure
{
  public:
    PhaseConfigure()
        : mChainFunction( nullptr )
    {
    }

    template <IsSchedulePhase... T>
    static PhaseConfigure Create()
    {
        return PhaseConfigure().Add<T...>();
    }

    /**
     * @brief T -> self, 先执行T，后执行self
     */
    template <IsSchedulePhase T>
    PhaseConfigure& After();

    /**
     * @brief self -> T , 先执行 Self，后执行 T
     */
    template <IsSchedulePhase T>
    PhaseConfigure& Before();

    PhaseConfigure& Chain();

    PhaseConfigure Build()
    {
        return *this;
    }

    void Apply( std::shared_ptr<class ScheduleManager> scheduleManager );

  protected:
    template <IsSchedulePhase... T>
    PhaseConfigure& Add();

  protected:
    std::vector<std::function<PhaseId( std::shared_ptr<ScheduleManager> )>> mCurNodesFunctions;
    std::vector<std::function<void( std::shared_ptr<ScheduleManager>, PhaseId )>> mOperateFunctions;
    std::function<void( std::shared_ptr<ScheduleManager>, PhaseIdChainType&& )> mChainFunction = nullptr;
};

#include "ecs/schedule/schedule_manager.h"

template <IsSchedulePhase... T>
PhaseConfigure& PhaseConfigure::Add()
{
    ( mCurNodesFunctions.push_back(
          []( std::shared_ptr<ScheduleManager> scheduleManager ) -> PhaseId
          {
              return scheduleManager->AddPhaseInConfig<T>();
          } ),
      ... );
    return *this;
}

/**
 * @brief T -> self, 先执行T，后执行self
 */
template <IsSchedulePhase T>
PhaseConfigure& PhaseConfigure::After()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<ScheduleManager> scheduleManager, PhaseId curNodeId )
        {
            scheduleManager->AfterPhaseInConfig<T>( curNodeId );
        } );
    return *this;
}

/**
 * @brief self -> T , 先执行 Self，后执行 T
 */
template <IsSchedulePhase T>
PhaseConfigure& PhaseConfigure::Before()
{
    mOperateFunctions.push_back(
        []( std::shared_ptr<ScheduleManager> scheduleManager, PhaseId curNodeId )
        {
            scheduleManager->BeforePhaseInConfig<T>( curNodeId );
        } );
    return *this;
}