
target("Window")
    set_kind("binary")
    add_files("**.cpp")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy")
    add_packages("vulkansdk")

    add_deps("nayuki_yq")
    add_deps("core")
    add_deps("render_core")