
module_name = "NayukiYq"
module_name_macro = "NAYUKI_YQ"

target(module_name)
    set_kind("shared")

    add_files("Src/" .. module_name .. "/**.cpp")
    add_includedirs("Include", { public = true })
    add_headerfiles("Include/**.h", { public = true })

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("glfw", "glm", "stb", "spdlog", "tracy")
    add_packages("vulkansdk")

    -- 添加本地target依赖
    add_deps("Core")
    add_deps("RenderCore")