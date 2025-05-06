module_name = "RenderCore"
module_name_macro = "RENDER_CORE"

target(module_name)
    set_kind("shared")

    add_files("Src/" .. module_name .. "/**.cpp", { pch = true })
    add_includedirs("Include", { public = true })
    add_headerfiles("Include/**.h", { public = true })

    set_pcxxheader("Include/" .. module_name .. "/" .. module_name .. ".h")

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy")
    add_packages("vulkansdk")

    add_deps("Core")