
module_name = "render"
module_name_macro = "RENDER"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    -- add_headerfiles("include/**.h", { public = true })

    -- local pheader_file = path.join("include", module_name, module_name .. ".h");
    --  set_pcxxheader(pheader_file)

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_deps("core")
    add_deps("render_core")
