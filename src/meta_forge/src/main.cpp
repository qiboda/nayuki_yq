#include "meta_forge/forge.h"

int main( int argc, const char** argv )
{
    Forge forge;
    forge.Init( argc, argv );
    forge.RunPhase( ForgePhase::ParseCommands );
    forge.Clear();
    return 0;
}
