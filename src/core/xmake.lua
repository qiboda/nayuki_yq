module_name = "core"
module_name_macro = "CORE"

target(module_name)
    set_kind("shared")

    set_group("libraries")

    add_files("src/**.cpp")
    -- public 意味着依赖这个 target 的其他 target 也会使用这个 includedirs。
    add_includedirs("include", { public = true })
    -- 文件会显示在 vs项目中的头文件目录下。并且install的时候会被安装到 include 目录下。
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

    add_packages("spdlog", "tracy", "glm", "rpmalloc", "tbb")

    after_build(function (target)
        for _, pkg in pairs(target:pkgs()) do
            local installdir = pkg:installdir()
            if installdir then
                -- 查找并拷贝所有 DLL/SO/DYLIB 文件
                local files = os.files(path.join(installdir, "bin/*"))
                for _, f in ipairs(files) do
                    if f:endswith(".dll") or f:endswith(".so") or f:endswith(".dylib") then
                        os.cp(f, target:targetdir())
                        print("on package <" .. pkg:name() .. "> copy " .. f .. " to " .. target:targetdir())
                    end
                end
            end
        end
    end)