#pragma once

#include "ecs/systems/system.h"

class ECS_API SystemManager : public NonCopyable
{
  public:
    SystemManager();

  public:
    template <IsSystemConcept Func>
    SystemId AddSystem( Func func )
    {
        auto systemId = SystemIdRegistry::Get( func );
        if ( systemId != SystemId::Invalid )
        {
            // System already exists.
            return systemId;
        }

        auto newSystemId = SystemIdRegistry::Add( func );
        mSystems.emplace( newSystemId, std::make_unique<System<Func>>( func ) );
        return newSystemId;
    }

    const std::unique_ptr<ISystem>& GetSystem( SystemId systemId ) const
    {
        const auto it = mSystems.find( systemId );
        if ( it != mSystems.cend() )
        {
            return it->second;
        }
        return sNullISystem;
    }

    void RunSystem( class Registry* registry )
    {
        for ( auto& system : mSystems )
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