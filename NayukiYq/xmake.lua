
add_requires("glfw 3.4", "glm 1.0.1", "stb 2024.06.01", "flecs v4.0.3", "spdlog v1.15.0", "tracy v0.11.1", { config = {
    debug = true,
    shared = true,
}})
add_requires("vulkansdk", { system = true })

target("NayukiYq")
    set_kind("shared")

    add_files("Src/NayukiYq/**.cpp")
    add_includedirs("Include", { public = true })

    add_defines("NAYUKIYQ_EXPORTS")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("glfw", "glm", "stb", "flecs", "spdlog", "tracy")
    add_packages("vulkansdk")