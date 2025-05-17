includes(os.projectdir() .. "/xmake/utils.lua")

-- add_requires("stdexec", {configs= { toolchains = "clang" }})
add_requires("stdexec")

-- external = true -> /external:I to refer to the header file path 
add_requires("glfw", "glm", "stb", "spdlog", "tracy", "shaderc", "entt", {
    debug = true,
    external = true,
})
add_requires("vulkansdk", { system = true })

includes_cur_dirs()

-- before_build(function ()

-- end)
