
target("window")
    set_kind("binary")
    add_files("**.cpp")

    set_policy("build.c++.modules", true)

    -- set_default(false)
    set_group("examples")

    add_deps("nayuki")
    add_deps("render_core")