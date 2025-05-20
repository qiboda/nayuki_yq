
module_name = "nayuki_yq"
module_name_macro = "NAYUKI_YQ"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- set_pcxxheader("include/".. module_name .. "/" .. module_name .. ".h")

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy", "rpmalloc", "tbb")
    add_packages("vulkansdk")

    -- 添加本地target依赖
    add_deps("core")
    add_deps("render_core")

    -- set_toolchains("mingw@llvm-mingw")