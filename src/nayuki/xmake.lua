
module_name = "nayuki"
module_name_macro = "NAYUKI"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    set_policy("build.c++.modules", true)

    add_files("module/**.ixx", { public = true })
    add_files("src/**.cpp")
    -- add_includedirs("include", { public = true })
    -- add_headerfiles("include/**.h", { public = true })

    --  set_pcxxheader("include/".. module_name .. "/" .. module_name .. ".h")

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    -- 添加本地target依赖
    add_deps("core")
    add_deps("render_core")
