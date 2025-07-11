includes(os.projectdir() .. "/xmake/utils.lua")

add_requires("stdexec", { configs = { shared = true }, debug = true })

add_requires("imgui", { configs = { shared = true}, debug = true })

-- external = true -> /external:I to refer to the header file path
add_requires("glfw", "glm", "stb", "rpmalloc", {})

add_requires("fmt")
add_requires("glm", { configs = { modules = true }, debug = true })

add_requires("shaderc", { configs = { shared = false }, debug = true })
add_requires("tbb", { configs = { shared = true }, debug = true})
add_requires("tracy", { configs = { shared = true, manual_lifetime = true, delayed_init = true }, debug = true})
-- add_requires("range-v3", { configs = { shared = false }, debug = true})
add_requires("spdlog", { configs = { fmt_external = true }, debug = true})

add_requires("vulkansdk", { system = true })

add_requires("icu4c", { configs = { shared = true }, debug = true})

-- if has_config("test") then
add_requires("gtest", { configs = { main = false, shared = false, gmock = true } })
-- end

includes_cur_dirs()

-- 递归的包含所有的tests 模块
includes("**/tests/xmake.lua")

after_load(function ()
    import("core.project.task").run("module_export")
end)
