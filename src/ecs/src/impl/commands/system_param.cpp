
module ecs;

import :command;

Commands Commands::From( Registry* registry, State& state )
{
    state.Init( registry, registry->GetCommandManager() );
    return Commands( state );
}