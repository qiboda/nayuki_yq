#include "meta_forge/forge.h"

//int main( int argc, const char** argv )
int main()
{
    Forge forge;

    int argc_o = 3;
    const char** argv_o = new const char*[argc_o];
    argv_o[0] = "D:/nayuki_yq/build/windows/x64/debug/meta_forge.exe"; // Program name
    argv_o[1] = "--compilations=D:/nayuki_yq/.vscode";
    argv_o[2] = "D:/nayuki_yq/src/meta_forge";

    forge.Init( argc_o, argv_o );

    forge.RunPhase( ForgePhase::ParseCommands );

    forge.Clear();

    return 0;
}