module_name = "render_core"
module_name_macro = "RENDER_CORE"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    set_policy("build.c++.modules", true)

    -- modules
    add_files("src/impl/**.mpp", { kind = "source" })
    add_files("module/**.ixx", { public = true })
    -- 常规方案
    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- local pheader_file = path.join("include", module_name, module_name .. ".h");
    --  set_pcxxheader(pheader_file)

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_packages("shaderc")
    add_packages("glfw", {public = true})
    add_packages("vulkansdk", { public = true})

    add_deps("core")
