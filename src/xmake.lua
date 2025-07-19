includes(os.projectdir() .. "/xmake/utils.lua")

add_requires("stdexec", { configs = { }, debug = true })

-- external = true -> /external:I to refer to the header file path
add_requires("glfw", "glm", "stb", "rpmalloc", {})

add_requires("imgui", { configs = { vulkan = true, glfw = true }, debug = true })

add_requires("fmt")
add_requires("glm", { configs = { modules = true }, system = false, debug = true })

add_requires("shaderc", { configs = { }, debug = true })
add_requires("tbb", { configs = { }, debug = true})
add_requires("tracy", { configs = { manual_lifetime = true, delayed_init = true }, debug = true})
-- add_requires("range-v3", { configs = { }, debug = true})
add_requires("spdlog", { configs = { fmt_external = true }, debug = true})

add_requires("vulkan-loader", { system = true })
add_requires("vulkan-headers", { system = true })

add_requires("icu4c", { configs = {}, debug = true})

-- if has_config("test") then
add_requires("gtest", { configs = { main = false, gmock = true }, system = false })
-- end

includes_cur_dirs()

-- 递归的包含所有的tests 模块
includes("**/tests/xmake.lua")

after_load(function ()
    import("core.project.task").run("module_export")
end)
