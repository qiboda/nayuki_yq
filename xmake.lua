set_project("nayuki_yq")

set_xmakever("2.9.2")

set_version("0.1.0")

add_rules("mode.debug", "mode.release", "mode.releasedbg")
includes(os.projectdir() .. "/xmake/rules/llvm-cov.lua")

set_installdir("install")

-- 自动更新 compile commands 文件
add_rules("plugin.vsxmake.autoupdate")
add_plugindirs("xmake/plugins")

if is_host("windows") then
    set_toolchains("msvc")
else 
    set_toolchains("clang")
end

set_warnings("allextra", "error")
set_languages("c17", "cxx20")

-- 检查toolchains是msvc还是clang, 可能得到nil
-- if get_config("toolchain") == "msvc" 

add_cxxflags("cl::/utf-8")
-- 检测到有类型没有dll导出，但是被用在了dll接口中时报警告。
-- 由于标准库也会报警，太烦人了。禁用。
add_cxxflags("cl::/wd4251")
-- 允许隐式的删除赋值和拷贝构造函数。
add_cxxflags("cl::/wd4625")
add_cxxflags("cl::/wd4626")
-- 允许指定对齐
add_cxxflags("cl::/wd4324")
add_cxxflags("cl::/wd4820")
-- 允许没有使用的函数
add_cxxflags("cl::/wd4514")

add_cxxflags("clang::-ferror-limit=0")
-- 强制区分大小写
add_cxxflags("clang::-Wnonportable-include-path")

add_cxxflags("clang::-Wno-gnu-zero-variadic-macro-arguments")
-- 允许在构造函数中覆盖成员变量
add_cxxflags("clang::-Wno-shadow-field-in-constructor")
-- 允许使用static 全局变量
add_cxxflags("clang::-Wno-exit-time-destructors")
add_cxxflags("clang::-Wno-global-constructors")
-- 为了使用 #pragma system_header
add_cxxflags("clang::-Wno-pragma-system-header-outside-header")
add_cxxflags("clang::-Wno-newline-eof")
-- 允许没有使用的宏
add_cxxflags("clang::-Wno-unused-macros")
-- 由于使用了vulkan等依赖库, 允许使用旧风格的类型转换
add_cxxflags("clang::-Wno-old-style-cast")
-- 允许没有default分支
add_cxxflags("clang::-Wno-switch-default")
-- 关闭所有与旧标准兼容性相关的警告（C++98 到 C++17）
add_cxxflags(
    "clang::-Wno-c++98-compat",
    "clang::-Wno-c++98-compat-pedantic",
    "clang::-Wno-c++11-compat",
    "clang::-Wno-c++11-compat-pedantic",
    "clang::-Wno-c++14-compat",
    "clang::-Wno-c++14-compat-pedantic",
    "clang::-Wno-c++17-compat",
    "clang::-Wno-c++17-compat-pedantic"
)

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
