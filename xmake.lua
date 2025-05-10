set_project("NayukiYq")

set_xmakever("2.9.2")

if is_host("windows") then
    set_toolchains("clang-cl")
elseif is_host("linux") then
    set_toolchains("clang")
elseif is_host("macosx") then
    set_toolchains("clang")
end

set_languages("c++20")

add_rules("mode.debug", "mode.release")
-- 自动更新 compile commands 文件
add_rules("plugin.vsxmake.autoupdate")

add_cxxflags("-Wall", "-Werror")
add_cxxflags("-ferror-limit=0")

if is_host("windows") then

    -- 允许在构造函数中覆盖成员变量
    add_cxxflags("-Wno-shadow-field-in-constructor")
    -- 允许使用static 全局变量
    add_cxxflags("-Wno-exit-time-destructors")
    add_cxxflags("-Wno-global-constructors")
    -- 为了使用 #pragma system_header
    add_cxxflags("-Wno-pragma-system-header-outside-header")
    add_cxxflags("-Wno-newline-eof")
    -- 允许没有使用的宏
    add_cxxflags("-Wno-unused-macros")
    -- 由于使用了vulkan等依赖库, 允许使用旧风格的类型转换
    add_cxxflags("-Wno-old-style-cast")
    -- 允许没有default分支
    add_cxxflags("-Wno-switch-default")
    -- 关闭所有与旧标准兼容性相关的警告（C++98 到 C++17）
    add_cxxflags(
        "-Wno-c++98-compat",
        "-Wno-c++98-compat-pedantic",
        "-Wno-c++11-compat",
        "-Wno-c++11-compat-pedantic",
        "-Wno-c++14-compat",
        "-Wno-c++14-compat-pedantic",
        "-Wno-c++17-compat",
        "-Wno-c++17-compat-pedantic"
    )
end 

if is_mode("debug") then
    add_defines("DEBUG")
else
    add_defines("NDEBUG")
end

-- add engine
includes("Src")

-- add examples
includes("Examples")