set_project("nayuki_yq")

set_xmakever("2.9.2")

set_version("0.1.0")

add_requires("llvm-mingw")

-- if is_host("windows") then
--     set_toolchains("mingw@llvm-mingw")
-- elseif is_host("linux") then
--     set_toolchains("mingw@llvm-mingw")
-- elseif is_host("macosx") then
--     set_toolchains("mingw@llvm-mingw")
-- end
-- 

set_languages("c++20")

add_rules("mode.debug", "mode.release", "mode.releasedbg")

includes(os.projectdir() .. "/xmake/rules/llvm-cov.lua")
add_rules("mode.llvm.coverage")
add_rules("mode.coverage")

set_installdir("install")

-- 自动更新 compile commands 文件
add_rules("plugin.vsxmake.autoupdate")
add_plugindirs("xmake/plugins")

-- 检查toolchains是msvc还是clang
if is_host("windows") then

else
    add_cxxflags("-Wall", "-Werror") -- "-Werror"
    add_cxxflags("-ferror-limit=0")
    -- 强制区分大小写
    add_cxxflags("-Wnonportable-include-path")

    if is_host("windows") then
        add_cxxflags("-Wno-gnu-zero-variadic-macro-arguments")
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
end

if is_mode("debug") then
    add_defines("DEBUG")
else
    -- 发行版模式 asset 不会执行
    add_defines("NDEBUG")
end

-- add engine
includes("src")

-- add examples
includes("examples")
