
module_name = "meta_core"
module_name_macro = "META_CORE"

target(module_name)
    set_kind("static")

    set_group("libraries")

    add_files("module/**.ixx", { public = true })
    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_deps("core")