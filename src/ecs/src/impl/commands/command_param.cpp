
module ecs.commands.system;
import ecs.registry;

Commands Commands::From( Registry* registry, State& state )
{
    state.Init( registry->mRegistryContext, registry->mCommandManager );
    return Commands( state );
}