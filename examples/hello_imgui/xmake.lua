
target("hello_imgui")
    set_kind("binary")
    add_files("**.cpp")

    set_default(false)
    set_group("examples")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy", "imgui")
    add_packages("vulkansdk")

    add_deps("nayuki_yq")
    add_deps("core")
    add_deps("render_core")