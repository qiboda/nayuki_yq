
module ecs.registry;

import ecs.commands;
import ecs.registry.context;

Registry::Registry()
    : mRegistryContext( new RegistryContext() )
    , mFeatureManager( new FeatureManager() )
    , mCommandManager( new CommandManager() )
{
}
