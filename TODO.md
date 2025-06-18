
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
