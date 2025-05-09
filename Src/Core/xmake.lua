module_name = "Core"
module_name_macro = "CORE"

target(module_name)
    set_kind("shared")

    add_files("Src/" .. module_name .. "/**.cpp")
    -- public 意味着依赖这个 target 的其他 target 也会使用这个 includedirs。
    add_includedirs("Include", { public = true })
    add_headerfiles("Include/**.h", { public = true })

    -- 预编译头文件
    set_pcxxheader("Include/".. module_name .. "/" .. module_name .. ".h")

    -- 启用dll export
    add_defines(module_name_macro .. "_EXPORTS")

    -- include 自动生成的文件。public 意味着依赖这个 target 的其他 target 也会使用这个 includedirs。
    local gen_dir = path.join(os.projectdir(), "build", "generated")
    add_includedirs(gen_dir, { public = true })

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("spdlog", "tracy", "glm")
