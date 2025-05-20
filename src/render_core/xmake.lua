module_name = "render_core"
module_name_macro = "RENDER_CORE"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- local pheader_file = path.join("include", module_name, module_name .. ".h");
    -- set_pcxxheader(pheader_file)

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy", "shaderc", "rpmalloc", "tbb")
    add_packages("vulkansdk")

    add_deps("core")

    -- set_toolchains("mingw@llvm-mingw")