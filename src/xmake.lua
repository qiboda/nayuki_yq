includes(os.projectdir() .. "/xmake/utils.lua")

-- , {configs= { toolchains = "clang" }}
add_requires("stdexec")

-- external = true -> /external:I to refer to the header file path 
add_requires("glfw", "glm", "stb", "spdlog", "rpmalloc", {
    debug = true,
    external = true,
})
add_requires("shaderc", { configs = { shared = false }, debug = true })
add_requires("tbb", { configs = { shared = true }, debug = true})
add_requires("tracy", { configs = { shared = true, manual_lifetime = true, delayed_init = true }, debug = true})
add_requires("range-v3", { configs = { shared = false }, debug = true})

add_requires("vulkansdk", { system = true })

-- if has_config("test") then
add_requires("gtest", { configs = { main = false, shared = false, gmock = true } })
-- end

includes_cur_dirs()

-- 递归的包含所有的tests 模块
includes("**/tests/xmake.lua")

before_build(function ()
    import("core.project.task").run("module_export")
end)
