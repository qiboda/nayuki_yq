
2. use vulkan
3. use gltf2.0
4. use glfw glm
5. use tracy to profiler
6. use spdlog
7. use google-test

error: LINK : fatal error LNK1181: cannot open input file 'RenderCore.lib'
可能是因为没有dll export 生成。例如，所有class 都没有加CORE_API。

只有头文件的部分，必须被包含在cpp文件中，否则，不会被编译进dll文件中，可能link错误。

// 操作还是有一点点危险。
因为全局重载了new，使用了全新的申请器。
所以，如果惰性 static 变量内部有new，会使用全局的new，
但是程序退出时，全局重载的new内部的申请器已经释放了，static变量没有办法正常free。

clangd 不能和cl共同使用

todo

1. 使用clang-libtooling 解析ast，得到反射信息
2. 使用有命名空间的attribute，来作为序列化，反射等标记。
3. 实现反射功能。
4. Component的序列化依赖反射系统。
5. 基于反射或者静态反射实现序列化 类似reflect-cpp
6. 基于序列化, 单个组件使用json？使用DuckDB，来导出archetype chu的数据到数据库，以及支持持久化和可视化Debug

lang-cl -### /c .\src\meta_forge\src\meta_forge.cpp 
clang version 19.1.5
Target: i686-pc-windows-msvc
Thread model: posix
InstalledDir: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin
 (in-process)
 "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\Llvm\\bin\\clang-cl.exe" "-cc1" "-triple" "i686-pc-windows-msvc19.44.35207" "-emit-obj" "-mincremental-linker-compatible" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "meta_forge.cpp" "-mrelocation-model" "static" "-mframe-pointer=all" "-relaxed-aliasing" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-fms-volatile" "-target-cpu" "pentium4" "-mllvm" "-x86-asm-syntax=intel" "-tune-cpu" "generic" "-D_MT" "-flto-visibility-public-std" "--dependent-lib=libcmt" "--dependent-lib=oldnames" "-stack-protector" "2" "-fdiagnostics-format" "msvc" "-fdebug-compilation-dir=D:\\nayuki_yq" "-fcoverage-compilation-dir=D:\\nayuki_yq" "-resource-dir" "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\Llvm\\lib\\clang\\19" "-internal-isystem" "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\Llvm\\lib\\clang\\19\\include" "-internal-isystem" "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" "-internal-isystem" "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\atlmfc\\include" "-internal-isystem" "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\ucrt" "-internal-isystem" "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\shared" "-internal-isystem" "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\um" "-internal-isystem" "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\winrt" "-internal-isystem" "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\cppwinrt" "-fdeprecated-macro" "-ferror-limit" "19" "-fmessage-length=194" "-fno-use-cxa-atexit" "-fms-extensions" "-fms-compatibility" "-fms-compatibility-version=19.44.35207" "-std=c++14" "-fskip-odr-check-in-gmf" "-fdelayed-template-parsing" "-fcolor-diagnostics" "-faddrsig" "-o" "meta_forge.obj" "-x" "c++" ".\\src\\meta_forge\\src\\meta_forge.cpp"



 xmake run meta_forge --compilations=D:/nayuki_yq/.vscode/ D:\nayuki_yq\src\meta_forgeo

用于反射，看情况，自己修改
https://github.com/Ubpa/UDRefl


向下转型，有运行时反射，可以根据反射来判断。