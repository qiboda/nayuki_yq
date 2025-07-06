/**
 * @brief
 * 步骤：
 * 1. 扫描指定模块的文件。
 * 2. 解析命令行参数。
 */
import meta_forge.forge;
import core.type;
import meta_forge.forge_phase;

// int main( int argc, const char** argv )
int main()
{
    Forge forge;

    usize argc_o = 2;
    const char** argv_o = new const char*[argc_o];
    argv_o[0] = "/home/skwy/repos/nayuki_yq/build/windows/x64/release/meta_forge"; // Program name
    argv_o[1] = "asset";

    forge.Init( argc_o, argv_o );

    forge.RunPhase( ForgePhase::LoadCompileArgs );

    forge.Clear();

    return 0;
}