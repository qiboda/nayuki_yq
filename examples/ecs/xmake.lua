target("ecs_start")
    set_kind("binary")
    add_files("**.cpp")

    -- set_default(false)
    set_group("examples")

    add_deps("core")
    add_deps("ecs")

    set_policy("build.c++.modules", true)