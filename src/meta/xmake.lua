
module_name = "meta"
module_name_macro = "META"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- local pheader_file = path.join("include", module_name, module_name .. ".h");
    -- set_pcxxheader(pheader_file)

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_deps("core")
