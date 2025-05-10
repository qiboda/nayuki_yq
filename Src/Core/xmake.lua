module_name = "core"
module_name_macro = "CORE"

target(module_name)
    set_kind("shared")

    add_files("src/**.cpp")
    -- public 意味着依赖这个 target 的其他 target 也会使用这个 includedirs。
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- 预编译头文件
    -- set_pcxxheader("include/".. module_name .. "/" .. module_name .. ".h")

    -- 启用dll export
    add_defines(module_name_macro .. "_EXPORTS")

    -- include 自动生成的文件。public 意味着依赖这个 target 的其他 target 也会使用这个 includedirs。
    local gen_dir = path.join(os.projectdir(), "build", "generated")
    add_includedirs(gen_dir, { public = true })

    -- 这两个选项同时使用，生成独立的debug符号信息。
    set_symbols("debug")
    set_strip("all")

    add_packages("spdlog", "tracy", "glm")

includes("Tests")