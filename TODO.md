
2. use vulkan
3. use gltf2.0
4. use glfw glm
5. use tracy to profiler
6. use spdlog
7. use google-test

error: LINK : fatal error LNK1181: cannot open input file 'RenderCore.lib'
可能是因为没有dll export 生成。例如，所有class 都没有加CORE_API。

只有头文件的部分，必须被包含在cpp文件中，否则，不会被编译进dll文件中，可能link错误。
