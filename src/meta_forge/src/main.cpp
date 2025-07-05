import meta_forge.forge;
import core.type;
import meta_forge.forge_phase;

// int main( int argc, const char** argv )
int main()
{
    Forge forge;

    usize argc_o = 3;
    const char** argv_o = new const char*[argc_o];
    argv_o[0] = "D:/nayuki_yq/build/windows/x64/debug/meta_forge.exe"; // Program name
    argv_o[1] = "--compilations=D:/nayuki_yq/.vscode";
    argv_o[2] = "D:/nayuki_yq/src/meta_forge/tests";

    forge.Init( argc_o, argv_o );

    forge.RunPhase( ForgePhase::ParseCommands );

    forge.Clear();

    return 0;
}