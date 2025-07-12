
module_name = "ecs"
module_name_macro = "ECS"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    set_policy("build.c++.modules", true)

    add_files("module/**.ixx", { public = true })
    add_files("src/**.cpp")
    -- add_includedirs("include", { public = true })
    -- add_headerfiles("include/**.h", { public = true })

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_deps("core")
    add_deps("meta")
