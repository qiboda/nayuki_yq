#pragma once

#include "ecs/systems/system.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <memory>

class ECS_API SystemManager
{
  public:
    SystemManager();

  public:
    template <IsSystemConcept Func>
    SystemId AddSystem( Func func )
    {
        auto systemId = SystemId();
        systemId.Set();
        mSystems.emplace( systemId, std::make_unique<System<Func>>( func ) );
        return systemId;
    }

    const std::unique_ptr<ISystem> &GetSystem( SystemId systemId ) const
    {
        const auto it = mSystems.find( systemId );
        if ( it != mSystems.cend() )
        {
            return it->second;
        }
        return sNullISystem;
    }

    void RunSystem( class Registry *registry )
    {
        for ( auto &system : mSystems )
        {
            system.second->Run( registry );
        }
    }

    void BuildSystemGraph()
    {
    }

  protected:
    std::map<SystemId, std::unique_ptr<ISystem>> mSystems;
};