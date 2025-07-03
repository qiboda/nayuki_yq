
target("hello_imgui")
    set_kind("binary")
    add_files("**.cpp")

    set_default(false)
    set_group("examples")

    add_packages("imgui")

    add_deps("nayuki_yq")
    add_deps("render_core")