
target("window")
    set_kind("binary")
    add_files("**.cpp")

    set_default(false)
    set_group("examples")

    add_deps("nayuki_yq")
    add_deps("render_core")