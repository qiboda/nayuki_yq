#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/container/small_vector.h>
#include <core/minimal.h>
#include <ecs/minimal.h>

/// a.before(b).after(c);
/// [a, b, c].before(d).after(e);
struct SchedulePhaseConfigure
{
    template <typename... T>
    SchedulePhaseConfigure()
        : mPhases( ( std::make_shared<T>(), ... ) )
    {
    }

    template <IsSchedulePhase T>
    SchedulePhaseConfigure& Before()
    {
        mBeforePhases.push_back( std::make_shared<T>() );
        return *this;
    }

    template <IsSchedulePhase T>
    SchedulePhaseConfigure& After()
    {
        mAfterPhases.push_back( std::make_shared<T>() );
        return *this;
    }

  protected:
    SmallVector<std::shared_ptr<SchedulePhaseBase>, 4> mBeforePhases;
    SmallVector<std::shared_ptr<SchedulePhaseBase>, 4> mAfterPhases;

    SmallVector<std::shared_ptr<SchedulePhaseBase>, 16> mPhases;
};

struct SchedulePhaseConfigureSet
{
  public:
    std::vector<std::shared_ptr<SchedulePhaseBase>> mPhases;
    // std::vector<Constraint> mConstraints;
};
